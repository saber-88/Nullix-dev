/* #pragma once */

#ifndef UNIQX_PD_HPP_UNIQX_
    #define UNIQX_PD_HPP_UNIQX_
    
    #include "globals.hpp"
    #include "sub_process/SubProcess.hpp"
    
    namespace [[
        
        ]] uniqx
    {
        
        class [[
            /* nullAttr_ */
        ]] system::process::
            SubProcess::UniqxPd
            {
                
                private: int PM_i_pd { -1 };
                private: enum class
                    PollStatus : short
                    {
                        _em_Data ,
                        _em_Sleep ,
                        _em_Closed ,
                    }
                ;
                
                /// default constructor
                public: CLASS_CTOR UniqxPd
                    ( void /* v_ */ )
                = default;
                
                /// delete copy constructor
                public: CLASS_CTOR UniqxPd
                    ( const UniqxPd& )
                = delete ( "disable copy" );
                
                /// delete copy operations
                public: auto operator =
                    ( const UniqxPd& )
                -> UniqxPd& = delete ( "disable copy" );
                
                /// explicit constructor with initializer
                public: explicit
                    CLASS_CTOR UniqxPd
                    ( const int /* k_i_pd_ */ )
                ;
                
                /// move constructor
                public: CLASS_CTOR UniqxPd
                    ( UniqxPd&& /* Upd_otherPd_ */ ) noexcept ( true )
                ;
                
                /// move operations
                public: auto operator =
                    ( UniqxPd&& /* Upd_otherPd_ */ ) noexcept ( true )
                -> UniqxPd&;
                
                /// release the owned pd to prevent destructor from firing
                auto
                    release
                    ( void /* v_ */ ) noexcept ( true )
                -> int;
                
                /// redirect owned pd
                auto
                    redirectTo
                    ( const int /* i_targetPd_ */ ) noexcept ( true )
                -> void;
                
                /// redirect owned pd
                auto
                    setNonBlock
                    ( const bool /* k_b_enabled_ */ ) noexcept ( true )
                -> void;
                
                /// redirect owned pd
                auto
                    pollState
                    ( const int /* k_i_tmoutMS_ */ ) noexcept ( true )
                -> PollStatus;
                
                /// read data from the owned pd
                auto
                    hasData
                    ( void /* v_ */ ) const noexcept ( true )
                -> bool;
                
                /// read data from the owned pd
                auto
                    getData
                    ( void /* v_ */ ) noexcept ( false )
                -> std::string;
                
                /// read data from the owned pd
                auto
                    writeDataTo
                    ( std::string& /* ref_str_buffer_ */ ) noexcept ( true )
                -> void;
                
                /// read data from the owned pd
                auto
                    flushDataTo
                    ( std::string& /* ref_str_resBufData_ */ )
                    noexcept ( true )
                -> bool;
                
                /// close the owned pd
                auto
                    close
                    ( void /* v_ */ ) noexcept ( true )
                -> bool;
                
                /// close the owned pd
                auto
                    closed
                    ( void /* v_ */ ) noexcept ( true )
                -> bool;
                
                /// conversion operator int
                operator int
                    ( void /* v_ */ ) const noexcept ( true )
                ;
                
                /// destructor for clean up
                public: CLASS_DTOR ~UniqxPd
                    ( void /* v_ */ ) noexcept ( true )
                ;
                
            }
        ;
        
    } /* namespace uniqx */
    
#endif /* UNIQX_PD_HPP_UNIQX_ */
