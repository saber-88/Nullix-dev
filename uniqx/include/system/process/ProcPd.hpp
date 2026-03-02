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
                private: int mutable PMm_i_pd { -1 };
                private: size_t mutable PM_zu_RX_Offset { };
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
                    ( ProcPd const& /* ProcPd */ )
                = delete ( "Do not copy pipe descriptor!" );
                
                /// explicit constructor with initializer
                public: explicit
                    CLASS_CTOR ProcPd
                    ( int const /* k_i_pd_ */ )
                ;
                
                /// move constructor
                public: CLASS_CTOR ProcPd
                    ( ProcPd&& /* Ppd_otherPd_ */ )
                    noexcept ( true )
                ;
                
                /// set the owned pd data read to non blocking
                public: auto setNonBlock
                    ( bool const /* k_b_enabled_ */ ) const
                    noexcept ( true )
                -> void;
                
                public: auto pollState
                    ( int const /* k_i_tmoutMS_ */ = { } ) const
                    noexcept ( true )
                -> PollStatus;
                
                /// close the owned pd
                public: auto close
                    ( void /* v_ */ ) const
                    noexcept ( true )
                -> void;
                
                /// check if owned pd is closed
                public: auto closed
                    ( void /* v_ */ ) const
                    noexcept ( true )
                -> bool;
                
                /// delete owned pd copy operation
                public: auto operator =
                    ( ProcPd const& /* ProcPd */ )
                -> ProcPd&
                = delete ( "Do not copy pipe descriptor!" );
                
                /// owned pd move operation
                public: auto operator =
                    ( ProcPd&& /* Ppd_otherPd_ */ ) const
                    noexcept ( true )
                -> ProcPd&;
                
                /// owned pd redirection operation
                public: auto operator >>=
                    ( int const /* i_targetPd_ */ ) const
                    noexcept ( true )
                -> void;
                
                /// owned pd TX data
                public: auto operator >>
                    ( std::string& /* r_str_resBufData_TX_ */ ) const
                    noexcept ( true )
                -> bool;
                
                /// owned pd RX Data
                public: auto operator <<
                    ( std::string const& /* kr_str_resBufData_RX_ */ ) const
                    noexcept ( true )
                -> bool;
                
                /// owned pd conversion operator int
                public: operator int
                    ( void /* v_ */ ) const
                    noexcept ( true )
                ;
                
                /// owned pd destructor for clean up
                public: CLASS_DTOR ~ProcPd
                    ( void /* v_ */ )
                    noexcept ( true )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* PROC_PD_HPP_UNIQX_ */
