/* #pragma once */

#ifndef PROC_PD_HPP_UNIQX_
    #define PROC_PD_HPP_UNIQX_
    
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
        ]] system::process::ProcPd
            {
                
                DIAGNOSTICS_PUSH
                DIAGNOSTICS_IGNORED ( "-Wpadded" )
                private: int PM_i_pd { -1 };
                private: size_t PM_zu_RX_Offset { };
                DIAGNOSTICS_POP
                
                private: enum class
                    PollStatus
                    {
                        _em_Data ,
                        _em_Sleep ,
                        _em_Closed ,
                    }
                ;
                
                /// default constructor
                public: CLASS_CTOR ProcPd
                    ( void /* v_ */ )
                = default;
                
                /// delete copy constructor
                public: CLASS_CTOR ProcPd
                    ( const ProcPd& /* ProcPd */ )
                = delete ( "Do not copy pipe descriptor!" );
                
                /// explicit constructor with initializer
                public: explicit
                    CLASS_CTOR ProcPd
                    ( const int /* k_i_pd_ */ )
                ;
                
                /// move constructor
                public: CLASS_CTOR ProcPd
                    ( ProcPd&& /* Ppd_otherPd_ */ ) noexcept ( true )
                ;
                
                /// set the owned pd data read to non blocking
                public: auto setNonBlock
                    ( const bool /* k_b_enabled_ */ ) noexcept ( true )
                -> void;
                
                public: auto pollState
                    ( const int /* k_i_tmoutMS_ */ = { } ) noexcept ( true )
                -> PollStatus;
                
                /// close the owned pd
                public: auto close
                    ( void /* v_ */ ) noexcept ( true )
                -> void;
                
                /// check if owned pd is closed
                public: auto closed
                    ( void /* v_ */ ) noexcept ( true )
                -> bool;
                
                /// delete owned pd copy operation
                public: auto operator =
                    ( const ProcPd& /* ProcPd */ )
                -> ProcPd&
                = delete ( "Do not copy pipe descriptor!" );
                
                /// owned pd move operation
                public: auto operator =
                    ( ProcPd&& /* Ppd_otherPd_ */ ) noexcept ( true )
                -> ProcPd&;
                
                /// owned pd redirection operation
                public: auto operator >>=
                    ( const int /* i_targetPd_ */ )
                    noexcept ( true )
                -> void;
                
                /// owned pd TX data
                public: auto operator >>
                    ( std::string& /* r_str_resBufData_TX_ */ )
                    noexcept ( true )
                -> bool;
                
                /// owned pd RX Data
                public: auto operator <<
                    ( const std::string& /* kr_str_resBufData_RX_ */ )
                    noexcept ( true )
                -> bool;
                
                /// owned pd conversion operator int
                public: operator int
                    ( void /* v_ */ ) const noexcept ( true )
                ;
                
                /// owned pd destructor for clean up
                public: CLASS_DTOR ~ProcPd
                    ( void /* v_ */ ) noexcept ( true )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* PROC_PD_HPP_UNIQX_ */
