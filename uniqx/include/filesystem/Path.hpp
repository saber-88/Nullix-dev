/* #pragma once */

#ifndef PATH_HPP_UNIQX_
    #define PATH_HPP_UNIQX_
    
    #include <expected>
    #include <filesystem>
    #include <initializer_list>
    
    #define ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    #
    #   include "uniqx/uniqx.hpp"
    #
    #undef ONLY_FORWARD_SYMBOL_DECLS_UNIQX_
    
    #include "globals.hpp"
    #include "error/error.hpp"
    
    namespace [[
            /* nullAttr_ */
        ]] uniqx
    {
        
        // namespace [[
        //         /* nullAttr_ */
        //     ]] filesystem
        // {
            
        //     auto safe_tmp_filename
        //         ( std::fs::path const& /* kr_fps_filePath_ */ )
        //         noexcept ( true )
        //     -> std::fs::path;
            
        // } /* namespace filesystem */
        
        class [[
            /* nullAttr_ */
        ]] filesystem::Path
                : public std::fs::path
            {
                
                /// @brief: std::fs::path constructors
                using std::fs::path::path;
                
                using iLfsp_t_ = std::initializer_list<std::fs::path const>;
                
                public: auto parent
                    ( void /* v_ */ ) const
                    noexcept ( false )
                -> uniqx::fs::Path;
                
                public: auto atomicLinkTo
                    (
                        std::fs::path const& /* kr_fsp_newDestPath_ */ ,
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                    noexcept ( false )
                -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>;
                
                public: auto unlink
                    (
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                    noexcept ( false )
                -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>;
                
                public: auto prune
                    (
                        iLfsp_t_ /* ilufP_boundaries_ */ ,
                        const std::src_loc /* k_sl_srcLoc_ */ =
                        { std::src_loc::current ( ) }
                    ) const
                    noexcept ( false )
                -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>;
                
                public: auto
                    operator /
                    ( std::fs::path const& /* kr_fsp_subDir_ */ ) const
                    noexcept ( false )
                -> uniqx::fs::Path;
                
                // public: auto
                //     operator /
                //     ( uniqx::fs::Path const& /* kr_ufP_subDir_ */ ) const
                //     noexcept ( false )
                // -> uniqx::fs::Path;
                
                // public: auto
                //     operator /
                //     ( std::string_view /* Sv_subDir_ */ ) const
                //     noexcept ( false )
                // -> uniqx::fs::Path;
                
                public: auto
                    operator >>=
                    (
                        std::fs::path const& /* kr_ufP_newDestPath_ */
                    ) const
                    noexcept ( false )
                -> std::expected<uniqx::fs::Path , errika::ErrInt_t_>;
                
            } /* class filesystem::Path */
        ;
        
    } /* namespace uniqx */
    
#endif /* PATH_HPP_UNIQX_ */

