#include <print>
#include <cerrno>
#include <ranges>
#include <cstddef>
#include <fcntl.h>
#include <utility>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <asm-generic/ioctls.h>

#include "globals.hpp"
#include "process/ProcPd.hpp"

namespace [[
        /* nullAttr_ */
    ]] uniqx
{
    
    using namespace system;
    
    CLASS_CTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::ProcPd
            ( const int k_i_pd_ )
        : PM_i_pd ( k_i_pd_ )
    { }
    
    CLASS_CTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::ProcPd
            ( ProcPd&& Ppd_otherPd_ ) noexcept
        : PM_i_pd ( std::exchange ( Ppd_otherPd_.PM_i_pd , -1 ) )
    { }
    
    auto process::
        ProcPd::operator =
        ( ProcPd&& Ppd_otherPd_ ) noexcept
    -> ProcPd&
    {
        
        if
            ( this != &Ppd_otherPd_ )
        {
            this->close ( );
            this->PM_i_pd = std::exchange ( Ppd_otherPd_.PM_i_pd , -1 );
        }
        
        return ( *this );
        
    }
    
    auto process::
        ProcPd::operator >>=
        ( const int i_targetPd_ ) noexcept ( true )
    -> void
    {
        
        if ( this->closed ( ) ) return;
        
        if
            (
                this->PM_i_pd != i_targetPd_ &&
                ::dup2 ( this->PM_i_pd , i_targetPd_ ) != -1
            ) [[ likely ]]
        {
            // success. so we can safely close it
            this->close ( );
            
        }
        
        else [[ unlikely ]]
        {
            // forget the descriptor
            std::println ( stderr , "failed to redirect fd" );
            // this->close ( );
            
        }
        
    }
    
    auto process::
        ProcPd::setNonBlock
        ( const bool k_b_enabled_ ) noexcept ( true )
    -> void
    {
        
        if ( this->closed ( ) ) [[ unlikely ]] return;
        
        if
            ( k_b_enabled_ ) [[ likely ]]
        {
            
            ::fcntl
                (
                    this->PM_i_pd ,
                    F_SETFL ,
                    ::fcntl ( this->PM_i_pd , F_GETFL , +0 ) | O_NONBLOCK
                )
            ;
            
        }
    }
    
    auto process::
        ProcPd::pollState
        ( const int k_i_tmoutMS_ ) noexcept ( true )
    -> PollStatus
    {
        
        using enum PollStatus;
        
        constexpr nfds_t k_nfd_pdLim { +1 };
        
        if ( this->closed ( ) ) [[ unlikely ]] return _em_Closed;
        
        pollfd
            _pfd_pd
            {
                .fd = this->PM_i_pd ,
                .events = POLLIN ,
                .revents = 0
            }
        ;
        
        if
            ( ::poll ( &_pfd_pd , k_nfd_pdLim , k_i_tmoutMS_ ) <= +0 )
        {
            return _em_Sleep;
        }
        
        if ( _pfd_pd.revents & POLLIN ) return _em_Data;
        if ( _pfd_pd.revents & POLLHUP ) return _em_Closed;
        
        return _em_Sleep;
    }
    
    auto process::
        ProcPd::operator >>
        ( std::string& r_str_resBufData_TX_ ) noexcept ( true )
    -> bool
    {
        using enum PollStatus;
        
        if ( this->closed ( ) ) [[ unlikely ]] return false;
        
        const PollStatus k_pst_state { this->pollState ( +0 ) };
        
        if
            ( k_pst_state == _em_Closed ) [[ unlikely ]]
        {
            
            this->close ( );
            return false;
            
        }
        
        if ( k_pst_state != _em_Data ) [[ unlikely ]] return false;
        
        // 64kb buffer
        constexpr size_t K_zu_pipeBufSize { +64 << +10 };
        const size_t K_zu_bufOffset { r_str_resBufData_TX_.size ( ) };
        
        r_str_resBufData_TX_.resize_and_overwrite
            (
                K_zu_bufOffset + K_zu_pipeBufSize ,
                [ & ] ( char* c_str_ , std::size_t zu_capacity_ )
                {
                    std::span<char>
                        _spn_bufView
                        {
                            std::views::counted
                            ( c_str_ , long ( zu_capacity_ ) )
                        }
                    ;
                    
                    auto _buf { _spn_bufView.subspan ( K_zu_bufOffset ) };
                    
                    const ssize_t
                        k_z_bytesRXd
                        {
                            ::read
                            (
                                this->PM_i_pd ,
                                _buf.data ( ) ,
                                _buf.size ( )
                            )
                        }
                    ;
                    
                    return
                        (
                            ( k_z_bytesRXd > 0 )
                            ? ( K_zu_bufOffset + size_t ( k_z_bytesRXd ) )
                            : K_zu_bufOffset
                        )
                    ;
                    
                }
                
            )
        ;
        
        return ( r_str_resBufData_TX_.size ( ) > K_zu_bufOffset );
        
    }
    
    auto process::
        ProcPd::operator <<
        ( const std::string& kr_str_resBufData_RX_ ) noexcept ( true )
    -> bool
    {
        
        if ( this->closed ( ) ) return false;
        
        if
            (
                this->PM_zu_RX_Offset >= kr_str_resBufData_RX_.size ( )
            )
        {
            
            this->close ( );
            
            return false;
            
        }
        
        std::span<const char> _spn_remBufView { kr_str_resBufData_RX_ };
        
        auto _remBufCnt { _spn_remBufView.subspan ( this->PM_zu_RX_Offset ) };
        
        const ssize_t
            k_z_bytesTXd
            {
                ::write
                (
                    this->PM_i_pd ,
                    _remBufCnt.data ( ) ,
                    _remBufCnt.size ( )
                )
            }
        ;
        
        if ( k_z_bytesTXd == -1 && errno == EAGAIN ) return false;
        
        if
            ( k_z_bytesTXd == -1 )
        {
            
            this->close ( );
            return false;
            
        }
        
        this->PM_zu_RX_Offset += size_t ( k_z_bytesTXd );
        
        if
            ( this->PM_zu_RX_Offset >= kr_str_resBufData_RX_.size ( ) )
        {
            this->close ( );
        }
        
        return true;
        
    }
    
    auto process::
        ProcPd::close
        ( void /* v_ */ ) noexcept ( true )
    -> void
    {
        
        if
            ( this->PM_i_pd >= +0 )
        {
            
            ::close ( this->PM_i_pd );
            
            this->PM_i_pd = -1L;
            
        }
        
    }
    
    auto process::
        ProcPd::closed
        ( void /* v_ */ ) noexcept ( true )
    -> bool
    {
        return this->PM_i_pd < +0;
    }
    
    process::
        ProcPd::operator int
        ( void /* v_ */) const noexcept ( true )
    {
        
        return ( this->PM_i_pd );
        
    }
    
    CLASS_DTOR [[
        /* nullAttr_ */
    ]] process::
        ProcPd::~ProcPd
        ( void /* v_ */ ) noexcept ( true )
    {
        
        this->close ( );
        
    }
    
} /* namespace uniqx */
