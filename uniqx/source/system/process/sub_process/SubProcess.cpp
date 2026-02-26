// #include <print>
// #include <poll.h>
// #include <string>
// #include <vector>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>

// #include "globals.hpp"
// #include "sys_proc/Result.hpp"
// #include "sys_proc/SysProc.hpp"
// #include "sys_proc/UniqxPd.hpp"
// #include "sys_proc/UniqxPipe.hpp"

// namespace [[
//         /* nullAttr_ */
//     ]]  uniqx
// {
    
//     CLASS_CTOR
//         SysProc::SysProc
//             (
//                 const std::string& k_ref_str_binName_ ,
//                 const std::vector<string_t>& k_ref_vecStr_argv_
//             )
//         : PM_str_command ( k_ref_str_binName_ )
//         , PM_vecStr_argv ( k_ref_vecStr_argv_ )
//     { }
    
//     auto [[
//         /* Execution engine */
//     ]] SysProc::
//         mt_Res_execute
//         ( void /* v_ */ ) const
//     -> Result_t
//     {
        
//         auto [[
//             /* (Output pipe descriptor) Rx , Tx */
//         ]] [ _outPipeDes_RX , _outPipeDes_TX ]
//             {
//                 UniqxPipe::create ( )
//             }
//         ;
        
//         auto [[
//             /* (Error Output pipe descriptor) Rx , Tx */
//         ]] [ _errPipeDes_RX , _errPipeDes_TX ]
//             {
//                 UniqxPipe::create ( )
//             }
//         ;
        
//         ::pid_t _pid { ::fork ( ) };
        
//         if
//             ( _pid < +0 )
//         { }
        
//         if
//             ( _pid == +0 )
//         {
            
//             /* the chaild process transmits the data to us */
            
//             /// close read/reciver (RX) unused pipes else it will hang the pipe
//             _outPipeDes_RX.close ( );
//             _errPipeDes_RX.close ( );
            
//             /// Child process writes/transmits (TX) data to stdout
//             _outPipeDes_TX.redirectTo ( STDOUT_FILENO );
//             /// Child process writes/transmits (TX) data to stderr
//             _errPipeDes_TX.redirectTo ( STDERR_FILENO );
            
//             const auto k_zu_argvMax { this->PM_vecStr_argv.size ( ) + 1ZU };
            
//             std::vector<char* > _args { };
//             _args.reserve ( k_zu_argvMax );
            
//             _args.emplace_back
//                 ( const_cast<char* > ( PM_str_command.data ( ) ) )
//             ;
            
//             for
//                 ( const auto& arg_ : this->PM_vecStr_argv )
//             {
//                 _args.emplace_back ( const_cast<char* > ( arg_.data ( ) ) );
//             }
            
//             _args.emplace_back ( nullptr );
            
//             ::execvp ( _args.front ( ) , _args.data ( ) );
            
//             ::_exit ( 127 );
            
//         }
        
//         // else
//         // {
            
//         //     // we are not transmitting any data. so close the write(TX) ends
//         //     _outPipeDes_TX.close ( );
//         //     _errPipeDes_TX.close ( );
            
//         //     int _i_status { };
            
//         //     ::waitpid ( _pid , &_i_status , +0 );
            
//         //     return
//         //         {
//         //             ._status =
//         //             (
//         //                 WIFEXITED ( _i_status )
//         //                 ? WEXITSTATUS ( _i_status )
//         //                 : -1
//         //             ) ,
//         //             ._stdout
//         //             {
//         //                 _outPipeDes_RX.hasData ( )
//         //                 ? _outPipeDes_RX.getData ( )
//         //                 : ""
//         //             } ,
//         //             ._stderr
//         //             {
//         //                 _errPipeDes_RX.hasData ( )
//         //                 ? _errPipeDes_RX.getData ( )
//         //                 : ""
//         //             }
//         //         }
//         //     ;
            
//         // }
        
//         // else {
//         //     // 1. Close TX ends so we can detect EOF on RX ends
//         //     _outPipeDes_TX.close();
//         //     _errPipeDes_TX.close();
        
//         //     std::string outStr, errStr;
//         //     bool outOpen = true, errOpen = true;
        
//         //     // 2. The Multi-Pipe Read Loop
//         //     while (outOpen || errOpen) {
//         //         std::println("{}", "else parent");
//         //         if (outOpen) {
//         //             auto chunk = _outPipeDes_RX.getData(); // Needs to be a real read()
//         //             if (chunk.empty() && !_outPipeDes_RX.hasData()) outOpen = false;
//         //             else outStr += chunk;
//         //         }
//         //         if (errOpen) {
//         //             auto chunk = _errPipeDes_RX.getData();
//         //             if (chunk.empty() && !_errPipeDes_RX.hasData()) errOpen = false;
//         //             else errStr += chunk;
//         //         }
//         //         // Small yield or use poll() here to avoid 100% CPU usage
//         //     }
        
//         //     // 3. ONLY THEN wait for the process to clean up
//         //     int _i_status { };
//         //     ::waitpid(_pid, &_i_status, 0);
        
//         //     return {
//         //         ._status = WIFEXITED(_i_status) ? WEXITSTATUS(_i_status) : -1,
//         //         ._stdout = outStr,
//         //         ._stderr = errStr
//         //     };
//         // }
        
//         else 
// {
//     #pragma clang diagnostic ignored "-Weverything"
    
//     // 1. Close TX ends IMMEDIATELY. 
//     // If the parent keeps TX open, the loop below will never see EOF.
//     _outPipeDes_TX.close();
//     _errPipeDes_TX.close();

//     std::string outStr, errStr;
    
//     // 2. Setup polling for the RX descriptors
//     pollfd fds[2];
//     fds[0].fd = _outPipeDes_RX;
//     fds[0].events = POLLIN;
//     fds[1].fd = _errPipeDes_RX;
//     fds[1].events = POLLIN;

//     while (true) {
//         // Wait for activity on either pipe
//         int ret = ::poll(fds, 2, -1); // -1 = wait forever until activity
//         if (ret <= 0) break;

//         char buffer[4096];

//         for (int i = 0; i < 2; ++i) {
//             if (fds[i].revents & (POLLIN | POLLHUP)) {
//                 ssize_t bytes = ::read(fds[i].fd, buffer, sizeof(buffer));
//                 if (bytes > 0) {
//                     (i == 0 ? outStr : errStr).append(buffer, bytes);
//                 } else if (bytes == 0) {
//                     // EOF reached for this pipe
//                     fds[i].fd = -1; 
//                 }
//             }
//         }

//         // Exit loop when both pipes are closed
//         if (fds[0].fd == -1 && fds[1].fd == -1) break;
//     }

//     // 3. Now that the pipes are closed, the child is definitely done or finishing
//     int _i_status { };
//     ::waitpid(_pid, &_i_status, 0);

//     return {
//         ._status = WIFEXITED(_i_status) ? WEXITSTATUS(_i_status) : -1,
//         ._stdout = outStr,
//         ._stderr = errStr
//     };
// }
        
//         // return { };
        
//     }
    
//     auto SysProc::create
//         (
//             const std::string& k_ref_str_binName_ ,
//             const std::vector<string_t>& k_ref_vecStr_argv_
//         )
//     -> Result_t
//     {
        
//         // const SysProc proc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        
//         // return SysProc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
//         // return proc.mt_Res_execute();
        
//         return SysProc(k_ref_str_binName_ , k_ref_vecStr_argv_).mt_Res_execute();
        
//     }
    
// } /* namespace uniqx */

#include <fcntl.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "globals.hpp"
#include "sub_process/UniqxPd.hpp"
#include "sub_process/UniqxPipe.hpp"
#include "sub_process/ProcResult.hpp"
#include "sub_process/SubProcess.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    using namespace system::process;
    
    auto system::process::operator |
        ( SubProcess::opt_t opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<SubProcess::opt_t>
                (
                    static_cast<T> ( opt_lhs_ ) | static_cast<T> ( opt_rhs_ )
                )
            )
        ;
    }
    
    auto system::process::operator &
        ( SubProcess::opt_t opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<SubProcess::opt_t>
                (
                    static_cast<T> ( opt_lhs_ ) & static_cast<T> ( opt_rhs_ )
                )
            )
        ;
    }
    
    auto system::process::operator |=
        ( SubProcess::opt_t& opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t&
    {
        opt_lhs_ = opt_lhs_ | opt_rhs_;
        return ( opt_lhs_ );
    }
    
    auto system::process::operator &=
        ( SubProcess::opt_t& opt_lhs_ , SubProcess::opt_t opt_rhs_ )
    -> system::process::SubProcess::opt_t&
    {
        opt_lhs_ = opt_lhs_ & opt_rhs_;
        return ( opt_lhs_ );
    }
    
    
    auto hasFlag
        ( const SubProcess::opt_t opt_IO_ )
    -> bool
    {
        using T = std::underlying_type_t<SubProcess::opt_t>;
        return
            (
                static_cast<T> ( opt_IO_ ) != 0
            )
        ;
    }
    
    CLASS_CTOR
        SubProcess::SubProcess
            (
                const std::string& k_ref_str_binName_ ,
                const std::vector<string_t>& k_ref_vecStr_argv_
            )
        : PM_str_command ( k_ref_str_binName_ )
        , PM_vecStr_argv ( k_ref_vecStr_argv_ )
    { }
    
    auto [[
        /* Execution engine */
    ]] SubProcess::
        mt_Res_execute
        ( const ProcOpt pOpt_IO_ ) const
    -> Result_t
    {
        
        auto [[
            /* (Output pipe descriptor) Rx , Tx */
        ]] [ _outPipeDes_RX ,
            _outPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        auto [[
            /* (Error Output pipe descriptor) Rx , Tx */
        ]] [ _errPipeDes_RX ,
            _errPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
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
            
            /// Child process writes/transmits (TX) data to stdout
            _outPipeDes_TX.redirectTo ( STDOUT_FILENO );
            /// Child process writes/transmits (TX) data to stderr
            _errPipeDes_TX.redirectTo ( STDERR_FILENO );
            
            const auto k_zu_argvMax { this->PM_vecStr_argv.size ( ) + 3ZU };
            
            std::vector<char* > _args { };
            _args.reserve ( k_zu_argvMax );
            
            _args.emplace_back
                ( const_cast<char* > ( PM_str_command.c_str ( ) ) )
            ;
            
            for
                ( const auto& arg_ : this->PM_vecStr_argv )
            {
                _args.emplace_back ( const_cast<char* > ( arg_.c_str ( ) ) );
            }
            
            _args.emplace_back ( nullptr );
            
            ::execvp ( _args.front ( ) , _args.data ( ) );
            
            ::_exit ( 127 );
            
        }
        
        else
        {
            
            // we are not transmitting any data. so close the write(TX) ends
            _outPipeDes_TX.close ( );
            _errPipeDes_TX.close ( );
            
            // set nonblocking to prevent freezing when reading large buffers
            _outPipeDes_RX.setNonBlock ( true );
            _errPipeDes_RX.setNonBlock ( true );
            
            std::string _str_outResData;
            std::string _str_errResData;
            
            while
                ( !_outPipeDes_RX.closed ( ) || !_errPipeDes_RX.closed ( ) )
            {
                
                [[ maybe_unused ]] bool b_active { };
                
                if
                    ( hasFlag ( pOpt_IO_ & ProcOpt::stderr ) ) [[ likely ]]
                {
                    b_active |= _errPipeDes_RX.flushDataTo ( _str_errResData );
                }
                
                else
                {
                    b_active |= _errPipeDes_RX.flushDataTo ( _str_errResData );
                    _str_errResData.clear ( );
                }
                
                if
                    ( hasFlag ( pOpt_IO_ & ProcOpt::stdout ) ) [[ likely ]]
                {
                    b_active |= _outPipeDes_RX.flushDataTo ( _str_outResData );
                }
                
                else
                {
                    b_active |= _outPipeDes_RX.flushDataTo ( _str_outResData );
                    _str_outResData.clear ( );
                }
                
                if
                    (
                        !b_active &&
                        (
                            !_outPipeDes_RX.closed ( ) ||
                            !_errPipeDes_RX.closed ( )
                        )
                    )
                {
                    ::poll ( nullptr , +0 , +10 );
                }
                
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
                        : ""
                    } ,
                    ._stderr
                    {
                        not _str_errResData.empty ( )
                        ? std::move ( _str_errResData )
                        : ""
                    }
                }
            ;
            
        }
        
        // return { };
        
    }
    
    auto SubProcess::create
        (
            const std::string& k_ref_str_binName_ ,
            const std::vector<string_t>& k_ref_vecStr_argv_
        )
    -> SubProcess
    {
        
        // const SubProcess proc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        
        // return SubProcess ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        // return proc.mt_Res_execute();
        
        return
            (
                SubProcess
                (
                    k_ref_str_binName_ ,
                    k_ref_vecStr_argv_
                )
            )
        ;
        
    }
    
    auto SubProcess::run
        ( void /* v_ */ ) const
    -> int
    {
        
        this->mut_PM_pRes_cmdResult =
            this->SubProcess::mt_Res_execute ( PM_pOpt_opt )
        ;
        
        return
            (
                this->mut_PM_pRes_cmdResult.exit_code ( )
            )
        ;
        
    }
    
} /* namespace uniqx */
