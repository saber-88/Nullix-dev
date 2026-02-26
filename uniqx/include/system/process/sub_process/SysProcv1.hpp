// /* #pragma once */

// #ifndef SYS_PROC_HPP_UNIQX_
//     #define SYS_PROC_HPP_UNIQX_
    
    
//     #include <string>
//     #include <vector>
    
//     #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
//     #
//     #   include "uniqx/uniqx.hpp"
//     #
//     #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
//     #include "globals.hpp"
    
//     namespace [[
//             /* nullAttr_ */
//         ]] uniqx
//     {
        
//         class [[
//             /* nullAttr_ */
//         ]] SysProc
//             {
                
//                 /// Result type
//                 private: struct Result;
                
//                 /// Pipe Descriptor Handler
//                 private: class UniqxPd;
//                 /// Pipe Descriptor Spawner
//                 private: struct UniqxPipe;
                
//                 private: std::string PM_str_command;
//                 private: std::string PM_str_inputData;
//                 private: std::vector<std::string> PM_vecStr_argv;
                
//                 public: using Result_t = Result;
//                 private: using string_t = std::string;
//                 // public: using Config_t = Config;
//                 // public: uint_fast8_t envVars { 1 << 0 };
//                 // public: uint_fast8_t noEnvVars { 1 << 1 };
//                 // public: uint_fast8_t pathSearch { 1 << 2 };
//                 // public: uint_fast8_t noPathSearch { 1 << 3 };
                
//                 /// Class Constructor
//                 public: explicit CLASS_CTOR SysProc
//                     (
//                         const std::string& /* k_ref_str_binName_ */ ,
//                         const std::vector<string_t>& /* k_ref_vecStr_argv_ */
//                     )
//                 ;
                
//                 /// Execution engine
//                 private: auto mt_Res_execute
//                     ( void /* v_ */ ) const
//                     // ( uint_fast8_t /* k_i_flags_ */ ) const
//                 -> Result_t;
                
//                 public: auto static
//                     create
//                     (
//                         const std::string& /* k_ref_str_binName_ */ ,
//                         const std::vector<string_t>& /* k_ref_vecStr_argv_ */
//                     )
//                 -> Result_t;
                
//             }
//         ;
        
//     } /* namespace uniqx */
    
// #endif /* SYS_PROC_HPP_UNIQX_ */

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
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] SysProc
            {
                
                /// Result type
                private: struct Result;
                
                /// Pipe Descriptor Handler
                private: class UniqxPd;
                /// Pipe Descriptor Spawner
                private: struct UniqxPipe;
                
                private: std::string PM_str_inputData;
                private: const std::string& PM_str_command;
                private: std::vector<std::string> PM_vecStr_argv;
                
                public: using Result_t = Result;
                private: using string_t = std::string;
                // public: using Config_t = Config;
                // public: uint_fast8_t envVars { 1 << 0 };
                // public: uint_fast8_t noEnvVars { 1 << 1 };
                // public: uint_fast8_t pathSearch { 1 << 2 };
                // public: uint_fast8_t noPathSearch { 1 << 3 };
                
                /// Class Constructor
                public: explicit CLASS_CTOR SysProc
                    (
                        const std::string& /* k_ref_str_binName_ */ ,
                        const std::vector<string_t>& /* k_ref_vecStr_argv_ */
                    )
                ;
                
                /// Execution engine
                private: auto mt_Res_execute
                    ( void /* v_ */ ) const
                    // ( uint_fast8_t /* k_i_flags_ */ ) const
                -> Result_t;
                
                public: auto static
                    create
                    (
                        const std::string& /* k_ref_str_binName_ */ ,
                        const std::vector<string_t>& /* k_ref_vecStr_argv_ */
                    )
                -> Result_t;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* SYS_PROC_HPP_UNIQX_ */
