/* #pragma once */

#ifndef SYS_PROC_HPP_UNIQX_
    #define SYS_PROC_HPP_UNIQX_
    
    #include <string>
    #include <vector>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "globals.hpp"
    #include "process/ProcStream.hpp"
    #include "process/ProcResult.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] system::process::SubProcess
            {
                
                private: using opt_t = ProcStream;
                private: using Result_t = ProcResult;
                private: using string_t = std::string;
                private: using vecStr_t = std::vector<std::string>;
                
                private: mutable opt_t PM_pOpt_opt;
                private: mutable string_t PM_str_command;
                private: mutable string_t PM_str_inputData;
                private: mutable vecStr_t mut_PM_vecStr_argv;
                private: mutable ProcResult mut_PM_pRes_cmdResult;
                
                /// Class Constructor
                public: explicit CLASS_CTOR SubProcess
                    (
                        const string_t& /* kr_str_binName_ */ ,
                        const vecStr_t& /* kr_vecStr_argv_ */
                    )
                ;
                
                /// Execution engine
                private: auto mt_Res_execute
                    ( const opt_t /* pOpt_IO_ */ = opt_t::status ) const
                -> Result_t;
                
                public: auto
                    static create
                    (
                        const string_t& /* kr_str_binName_ */ ,
                        const vecStr_t& /* kr_vecStr_argv_ */ = { }
                    )
                -> SubProcess;
                
                public: auto printArgs
                    ( void /* v_ */ ) const
                -> void;
                
                public: auto run
                    ( void /* v_ */ ) const
                -> int;
                
                public: auto capture
                    ( const opt_t opt_ ) const
                -> const SubProcess&;
                
                public: inline auto result
                    ( void /* v_ */ ) const
                -> const Result_t&
                {
                    return this->mut_PM_pRes_cmdResult;
                }
                
                public: auto operator ( )
                    ( const string_t& /* kr_str_argv_ */ ) const
                -> const SubProcess&;
                
                public: auto operator [ ]
                    ( const string_t& /* kr_str_argv_ */ ) const
                -> const SubProcess&;
                
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* SYS_PROC_HPP_UNIQX_ */
