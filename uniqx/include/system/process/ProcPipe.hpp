/* #pragma once */

#ifndef UNIQX_PIPE_HPP_UNIQX_
    #define UNIQX_PIPE_HPP_UNIQX_
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "process/ProcPd.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        struct [[
            /* nullAttr_ */
        ]] system::
            process::ProcPipe
            {
                
                /// @brief: pipe descriptor reciever/reader (RX)
                ProcPd pm_pipeDes_RX;
                
                /// @brief: pipe descriptor transmitter/writter (TX)
                ProcPd pm_pipeDes_TX;
                
                /// @brief: Factory ::create to create pipe descriptors
                auto static create
                    ( void /* v_ */ )
                -> ProcPipe;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* UNIQX_PIPE_HPP_UNIQX_ */
