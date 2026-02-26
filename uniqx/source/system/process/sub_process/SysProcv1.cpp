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
            
//             std::vector<char* > _args { };
//             _args.reserve ( this->PM_vecStr_argv.size ( ) + 1ZU );
            
//             _args.emplace_back
//                 ( const_cast<char* > ( PM_str_command.data ( ) ) )
//             ;
            
//             for
//                 ( const auto& arg_ : this->PM_vecStr_argv )
//             {
//                 _args.emplace_back ( const_cast<char* > ( arg_.data ( ) ) );
//             }
            
//             _args.emplace_back ( nullptr );
            
//             ::execvp ( this->PM_str_command.data ( ) , _args.data ( ) );
            
//         }
        
//         else
//         {
            
//             // we are not transmitting any data. so close the write(TX) ends
//             _outPipeDes_TX.close ( );
//             _errPipeDes_TX.close ( );
            
//             std::string str_stdOut { };
//             std::string str_stdErr { };
            
//             int _i_status { };
            
//             ::waitpid ( _pid , &_i_status , +0 );
            
//             if
//                 ( _outPipeDes_RX.hasData ( ) )
//             {
//                 str_stdOut = _outPipeDes_RX.getData ( );
//             }
            
//             if
//                 ( _errPipeDes_RX.hasData ( ) )
//             {
//                 str_stdErr = _errPipeDes_RX.getData ( );
//             }
            
//             return
//                 {
//                     ._status =
//                     (
//                         WIFEXITED ( _i_status )
//                         ? WEXITSTATUS ( _i_status )
//                         : -1
//                     ) ,
//                     ._stdout { str_stdOut } ,
//                     ._stderr { str_stdErr } ,
//                 }
//             ;
            
//         }
        
//         return { };
        
//     }
    
//     auto SysProc::create
//         (
//             const std::string& k_ref_str_binName_ ,
//             const std::vector<string_t>& k_ref_vecStr_argv_
//         )
//     -> SysProc
//     {
        
//         // const SysProc proc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        
//         return SysProc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        
//     }
    
// } /* namespace uniqx */

#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "globals.hpp"
#include "sys_proc/Result.hpp"
#include "sys_proc/SysProc.hpp"
#include "sys_proc/UniqxPd.hpp"
#include "sys_proc/UniqxPipe.hpp"

namespace [[
        /* nullAttr_ */
    ]]  uniqx
{
    
    CLASS_CTOR
        SysProc::SysProc
            (
                const std::string& k_ref_str_binName_ ,
                const std::vector<string_t>& k_ref_vecStr_argv_
            )
        : PM_str_command ( k_ref_str_binName_ )
        , PM_vecStr_argv ( k_ref_vecStr_argv_ )
    { }
    
    auto [[
        /* Execution engine */
    ]] SysProc::
        mt_Res_execute
        ( void /* v_ */ ) const
    -> Result_t
    {
        
        auto [[
            /* (Output pipe descriptor) Rx , Tx */
        ]] [ _outPipeDes_RX , _outPipeDes_TX ]
            {
                UniqxPipe::create ( )
            }
        ;
        
        auto [[
            /* (Error Output pipe descriptor) Rx , Tx */
        ]] [ _errPipeDes_RX , _errPipeDes_TX ]
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
            
            const auto k_zu_argvMax { this->PM_vecStr_argv.size ( ) + 1ZU };
            
            std::vector<char* > _args { };
            _args.reserve ( k_zu_argvMax );
            
            _args.emplace_back
                ( const_cast<char* > ( PM_str_command.data ( ) ) )
            ;
            
            for
                ( const auto& arg_ : this->PM_vecStr_argv )
            {
                _args.emplace_back ( const_cast<char* > ( arg_.data ( ) ) );
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
                        _outPipeDes_RX.hasData ( )
                        ? _outPipeDes_RX.getData ( )
                        : ""
                    } ,
                    ._stderr
                    {
                        _errPipeDes_RX.hasData ( )
                        ? _errPipeDes_RX.getData ( )
                        : ""
                    }
                }
            ;
            
        }
        
        // return { };
        
    }
    
    auto SysProc::create
        (
            const std::string& k_ref_str_binName_ ,
            const std::vector<string_t>& k_ref_vecStr_argv_
        )
    -> Result_t
    {
        
        // const SysProc proc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        
        // return SysProc ( k_ref_str_binName_ , k_ref_vecStr_argv_ );
        // return proc.mt_Res_execute();
        
        return SysProc(k_ref_str_binName_ , k_ref_vecStr_argv_).mt_Res_execute();
        
    }
    
} /* namespace uniqx */
