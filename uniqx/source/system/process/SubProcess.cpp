#include <print>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <sys/poll.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "globals.hpp"
#include "process/ProcPd.hpp"
#include "process/ProcPipe.hpp"
#include "process/ProcResult.hpp"
#include "process/SubProcess.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    using namespace system::process;
    
    CLASS_CTOR
        SubProcess::SubProcess
            (
                const string_t& kr_str_cmdName_ ,
                const vecStr_t& kr_vecStr_argv_
            )
        : PM_str_command ( kr_str_cmdName_ )
        , mut_PM_vecStr_argv ( kr_vecStr_argv_ )
    {
        this->mut_PM_vecStr_argv.reserve
            ( this->mut_PM_vecStr_argv.size ( ) +32ZU )
        ;
        
        this->mut_PM_vecStr_argv.emplace_back
            ( kr_str_cmdName_ )
        ;
        
        std::rotate
            (
                this->mut_PM_vecStr_argv.rbegin ( ) ,
                this->mut_PM_vecStr_argv.rbegin ( ) + 1ZU ,
                this->mut_PM_vecStr_argv.rend ( )
            )
        ;
        
    }
    
    auto [[
        /* Execution engine */
    ]] SubProcess::
        mt_Res_execute
        ( const opt_t pOpt_IO_ ) const
    -> Result_t
    {
        
        /*! (Input pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stdin pipes !*/
        auto [ _inPipeDes_RX , _inPipeDes_TX ] { ProcPipe::create ( ) };
        
        /*! (Output pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stdout pipes !*/
        auto [ _outPipeDes_RX , _outPipeDes_TX ] { ProcPipe::create ( ) };
        
        /*! (Error Output pipe descriptor) [Rx , Tx] !*/
        /*! structed bindings to initialize stderr pipes !*/
        auto [ _errPipeDes_RX , _errPipeDes_TX ] { ProcPipe::create ( ) };
        
        if
            ( not ( pOpt_IO_ & opt_t::stdin ) )
        {
            _inPipeDes_RX.close ( );
            _inPipeDes_TX.close ( );
        }
        
        if
            ( not ( pOpt_IO_ & opt_t::stdout ) )
        {
            _outPipeDes_RX.close ( );
            _outPipeDes_TX.close ( );
        }
        
        if
            ( not ( pOpt_IO_ & opt_t::stderr ) )
        {
            _errPipeDes_RX.close ( );
            _errPipeDes_TX.close ( );
        }
        
        ::pid_t _pid { ::fork ( ) };
        
        if
            ( _pid < +0 )
        { }
        
        if
            ( _pid == +0 )
        {
            
            /* the chaild process transmits the data to us */
            
            /// close read/reciver (RX) unused pipes else it will hang the pipe
            _outPipeDes_RX.close ( );
            _errPipeDes_RX.close ( );
            
            if
                ( ( pOpt_IO_ & opt_t::stdout ) )
            {
                /// Child process writes/transmits (TX) data to stdout
                _outPipeDes_TX >>= STDOUT_FILENO;
            }
            
            if
                ( ( pOpt_IO_ & opt_t::stderr ) )
            {
                /// Child process writes/transmits (TX) data to stderr
                _errPipeDes_TX >>= STDERR_FILENO;
            }
            
            const auto k_zu_argvMax { this->mut_PM_vecStr_argv.size ( ) + 3ZU };
            
            std::vector<char* > _args { };
            _args.reserve ( k_zu_argvMax );
            
            for
                ( const auto& arg_ : this->mut_PM_vecStr_argv )
            {
                _args.emplace_back ( const_cast<char* > ( arg_.c_str ( ) ) );
            }
            
            _args.emplace_back ( nullptr );
            
            ::execvp ( _args.front ( ) , _args.data ( ) );
            
            ::_exit ( +127 );
            
        }
        
        else
        {
            
            // we are not transmitting any data. so close the write(TX) ends
            _outPipeDes_TX.close ( );
            _errPipeDes_TX.close ( );
            
            // set nonblocking to prevent freezing when reading large buffers
            _outPipeDes_RX.setNonBlock ( true );
            _errPipeDes_RX.setNonBlock ( true );
            
            constexpr std::size_t K_zu_bufSize { /* +64 << +10 */ };
            
            string_t _str_outResData; _str_outResData.reserve ( K_zu_bufSize );
            string_t _str_errResData; _str_errResData.reserve ( K_zu_bufSize );
            
            while
                ( !_outPipeDes_RX.closed ( ) || !_errPipeDes_RX.closed ( ) )
            {
                
                _outPipeDes_RX >> _str_outResData;
                
                _errPipeDes_RX >> _str_errResData;
                
            }
            
            int _i_status { };
            
            ::waitpid ( _pid , &_i_status , +0 );
            
            return
                {
                    ._status =
                    (
                        WIFEXITED ( _i_status )
                        ? WEXITSTATUS ( _i_status )
                        : -1
                    ) ,
                    ._stdout
                    {
                        not _str_outResData.empty ( )
                        ? std::move ( _str_outResData )
                        : "???"
                    } ,
                    ._stderr
                    {
                        not _str_errResData.empty ( )
                        ? std::move ( _str_errResData )
                        : "???"
                    }
                }
            ;
            
        }
        
    }
    
    auto SubProcess::create
        (
            const string_t& kr_str_cmdName_ ,
            const vecStr_t& kr_vecStr_argv_
        )
    -> SubProcess
    {
        
        return
            (
                SubProcess
                (
                    kr_str_cmdName_ ,
                    kr_vecStr_argv_
                )
            )
        ;
        
    }
    
    auto SubProcess::printArgs
        ( void /* v_ */ ) const
    -> void
    {
        
        std::println
            (
                stderr ,
                "\nCommand: {:?}" ,
                this->mut_PM_vecStr_argv.front ( )
            )
        ;
        
        std::print ( stderr , "Argv: " );
        
        for
            (
                size_t idx { +1ZU } ,
                size = this->mut_PM_vecStr_argv.size ( );
                idx < size; ++idx
            )
        {
            
            if ( idx == size - 1ZU )
            {
                
                std::println
                    (
                        stderr ,
                        "[ {:?} ]" ,
                        this->mut_PM_vecStr_argv [ idx ]
                    )
                ;
                
            }
            else
            {
                
                std::print
                    (
                        stderr ,
                        "[ {:?} ] " ,
                        this->mut_PM_vecStr_argv [ idx ]
                    )
                ;
                
            }
        }
        
        std::println ( );
        
    }
    
    auto SubProcess::capture
        ( const opt_t opt_ ) const
    -> const SubProcess&
    {
        if
            ( !opt_ )
        {
            return ( *this );
        }
        
        this->PM_pOpt_opt |= opt_;
        
        return ( *this );
    }
    
    auto SubProcess::run
        ( void /* v_ */ ) const
    -> int
    {
        
        this->mut_PM_pRes_cmdResult =
            this->SubProcess::mt_Res_execute ( this->PM_pOpt_opt )
        ;
        
        return
            (
                this->mut_PM_pRes_cmdResult.exit_code ( )
            )
        ;
        
    }
    
    auto SubProcess::operator ( )
        ( const string_t& kr_str_cmdName_ ) const
    -> const SubProcess&
    {
        
        if
            ( this->PM_str_command == kr_str_cmdName_ )
        {
            return ( *this );
        }
        
        this->PM_str_command = kr_str_cmdName_;
        
        this->mut_PM_vecStr_argv.front ( ) = kr_str_cmdName_;
        
        return ( *this );
        
    }
    
    auto SubProcess::operator [ ]
        ( const string_t& kr_str_argv_ ) const
    -> const SubProcess&
    {
        
        if
            (
                const auto&
                    kr_aut_itr
                    {
                        std::find
                        (
                            this->mut_PM_vecStr_argv.begin ( ) + 1ZU ,
                            this->mut_PM_vecStr_argv.end ( ) ,
                            kr_str_argv_
                        )
                    }
                ; kr_aut_itr == this->mut_PM_vecStr_argv.end ( )
            )
        {
            this->mut_PM_vecStr_argv.emplace_back ( kr_str_argv_ );
        }
        
        return ( *this );
        
    }
    
} /* namespace uniqx */
