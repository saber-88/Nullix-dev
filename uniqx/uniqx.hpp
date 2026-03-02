#ifndef UNIQX_HPP_
    #define UNIQX_HPP_
    
    #include "globals.hpp"
    
    namespace [[
        
        ]] uniqx
    {
        
        /*********************************************************************!
        @brief The Safety Interlock for global state changes.
        * Passing this struct to a .perma() call signals Explicit
        * intent to modify the persistent directory roots of the Host.
        !*********************************************************************/
        struct MutateGlobalState;
        
        struct
            MutateGlobalState
            {
                public: explicit
                    STRUCT_CTOR MutateGlobalState
                    ( void /* v_ */ ) = default
                ;
            }
        ;
        
        /*********************************************************************!
        @brief: A Unix like filesystem tree layout to easily manage files
        * & perform operations
        @details: FileTree acts as the 'Single Source of Truth' for the
        * * filesystem layout.
        * * It handles the mapping of standard folders (config, cache, local)
        * * & the dotfile source repository (optional).
        !*********************************************************************/
        
        namespace [[
                /* nullAttr_ */
            ]] system
        {
            
            class FileTree;
            
            namespace [[
                    /* nullAttr_ */
                ]] process
            {
                
                struct [[ /* Process Config */ ]] ProcRules;
                struct [[ /* Process DataIO */ ]] ProcStream;
                class [[ /* Process Manager */ ]] SubProcess;
                class [[ /* SubProcess Piper */ ]] SubProcPipe;
                class [[ /* Pipe Descriptor Handler */ ]] ProcPd;
                struct [[ /* Pipe Descriptor Spawner */ ]] ProcPipe;
                struct [[ /* Process's Result Storage */ ]] ProcResult;
                
            }
            
        } /* namespace system */
        
    } /* namespace uniqx */
    
    /*************************************************************************!
    @brief: Prevents circular dependencies.
    @details: #define the macro to get only forward declared symbols of uniqx ,
    * then #undef to allow the #ifndef condition to be true rid ourselves of
    * circular dependencies.
    !*************************************************************************!
    * Usage Example:
    * ```cpp
    *     #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    *     #
    *     #   include "uniqx.hpp"
    *     #
    *     #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    * ```
    !*************************************************************************/
    #if !defined(ONLY_FORWARD_SYMBOL_DECLS_UNIQX_)
    # /* IWYU pragma: begin_exports */
    #   include "system/process/ProcPd.hpp"
    #   include "system/process/ProcPipe.hpp"
    #   include "system/process/ProcResult.hpp"
    #   include "system/process/SubProcess.hpp"
    #   include "system/file_tree/FileTree.hpp"
    #   include "system/file_tree/PathProxy.hpp"
    # /* IWYU pragma: end_exports */
    #endif /* ONLY_FORWARD_SYMBOL_DECLS_UNIQX_ */
    
#endif /* UNIQX_HPP_ */

#if 0
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
#endif
