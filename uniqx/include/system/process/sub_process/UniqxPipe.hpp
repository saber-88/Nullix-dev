/* #pragma once */

#ifndef UNIQX_PIPE_HPP_UNIQX_
    #define UNIQX_PIPE_HPP_UNIQX_
    
    #include "sub_process/UniqxPd.hpp"
    #include "sub_process/SubProcess.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        struct [[
            /* nullAttr_ */
        ]] system::process::
            SubProcess::UniqxPipe
            {
                
                /// pipe descriptor reciever/reader (RX)
                UniqxPd pipeDes_RX;
                
                /// pipe descriptor transmitter/writter (TX)
                UniqxPd pipeDes_TX;
                
                /// Factory ::create to create pipe descriptors
                auto static create
                    ( void /* v_ */ )
                -> UniqxPipe;
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* UNIQX_PIPE_HPP_UNIQX_ */
