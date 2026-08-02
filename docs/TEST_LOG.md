# TEST LOG - E2E browser-use run @ 2026-08-02T19:55:04Z

Run note: target=ALL

| ID | Section | Type | Status | Duration | Notes |
|----|---------|------|--------|----------|-------|
| B-01 | A | build | ✅ PASS | 0.6s | exit 0 - gcc -pipe -Wall clean |
| B-02 | A | build | ✅ PASS | 0.0s | exit 0 - CLI failure paths |
| B-03 | A | browser | ✅ PASS | 0.8s | Minted from content/metadatas.txt - fallback:[PASS] blog_card_count=26 in 26..26;[PASS] blog_title_anchors=26 in 26..26;[PASS] search_input_present=1≥1 |
| B-04 | A | browser | ✅ PASS | 2.3s | Header/footer injection - fallback:[PASS] about_has_skip_link=2≥1;[PASS] about_has_versioned_stylesheet=2≥1;[PASS] about_has_home_link=2≥1;[PASS] about_has_blogs_link=2≥1;[PASS] about_has_projects_link=2≥1;[PASS] about_has_about_link=2≥1;[PASS |
| B-05 | A | build | ✅ PASS | 0.6s | exit 0 - Markdown-to-HTML conversion |
| B-06 | A | build | ✅ PASS | 0.0s | exit 0 - mkdir -p public/assets + cp -R |
| B-07 | A | build | ✅ PASS | 0.0s | exit 0 - CSS-only minifier |
| B-08 | A | browser | ✅ PASS | 2.3s | RSS 2.0 feed - fallback:[PASS] feed_root_is_rss=1≥1;[PASS] feed_has_channel=1≥1;[PASS] feed_has_exactly_25_items=26 in 26..26;[PASS] feed_channel_title_present=27≥1;[PASS] feed_channel_link_present=27≥1;[PASS] feed_channel_description_p |
| B-09 | A | build | ✅ PASS | 0.0s | exit 0 - 9 placeholders rendered |
| B-10 | A | build | ✅ PASS | 0.0s | exit 0 - Tags/time persisted |
| B-11 | A | build | ✅ PASS | 0.0s | exit 0 - Bounded output-path derivation |
| B-12 | A | skip | ⏭️ SKIP | 0.0s | Builder exits strict (1) (skipped) |
| B-13 | A | build | ✅ PASS | 0.6s | exit 0 - Build reproducibility |
| F-01 | B | browser | ✅ PASS | 1.1s | Home heading animation - fallback:[PASS] home_h1_typing_class=1≥1;[PASS] home_h1_hi_text=1≥1;[PASS] home_css_uses_monospace=2≥1;[PASS] home_css_has_dark_terminal_vars=21≥1 |
| F-02 | B | browser | ✅ PASS | 1.1s | Single keydown listener - fallback:[PASS] home_has_terminal_input_id=1≥1;[PASS] home_has_terminal_output_id=1≥1;[PASS] home_js_keydown_listener=2≥1;[PASS] home_js_command_not_found_copy=1≥1 |
| F-03 | B | browser | ✅ PASS | 0.7s | clear empties the output - fallback:[PASS] home_js_clear_branch=1≥1;[PASS] home_js_innerHTML_reset=1≥1 |
| F-04 | B | skip | ⏭️ SKIP | 0.0s | Live GitHub data (best-effort) (skipped) |
| F-05 | B | skip | ⏭️ SKIP | 0.0s | WakaTime parse (best-effort) (skipped) |
| F-06 | B | browser | ✅ PASS | 0.6s | Case-mismatch echo - fallback:[PASS] home_js_toLowerCase_on_echo=1≥1;[PASS] home_echoes_normalized_command=7≥1 |
| F-07 | B | browser | ✅ PASS | 1.1s | Header navigation - fallback:[PASS] home_has_home_anchor=1≥1;[PASS] home_has_blogs_anchor=1≥1;[PASS] home_has_projects_anchor=1≥1;[PASS] home_has_about_anchor=1≥1 |
| F-08 | B | browser | ✅ PASS | 0.8s | Single-column blogs list rendering - fallback:[PASS] blogs_card_count=26 in 26..26;[PASS] blogs_title_anchor_links=26 in 26..26;[PASS] blogs_single_column_css=1≥1 |
| F-09 | B | browser | ✅ PASS | 1.5s | Live search filter + ESC - fallback:[PASS] blogs_has_search_input=1≥1;[PASS] blogs_has_search_count=1≥1;[PASS] blogs_uses_is_hidden_class=3≥1;[PASS] blogs_escape_key_handler=1≥1;[PASS] blogs_script_not_markdown_code=0 in 0..0 |
| F-10 | B | browser | ✅ PASS | 0.8s | About page - fallback:[PASS] about_returns_main_section=2≥1;[PASS] about_has_h1_or_substack=4≥1 |
| F-11 | B | browser | ✅ PASS | 4.8s | Project showcase - fallback:[PASS] projects_returns_main_section=2≥1;[PASS] projects_has_ten_nodes=11 in 11..11;[PASS] projects_has_shhx=1≥1;[PASS] projects_has_zollo=1≥1;[PASS] projects_has_ector=1≥1;[PASS] projects_has_s2c=1≥1;[PASS] pr |
| F-12 | B | browser | ✅ PASS | 1.8s | Wide blog post with generated TOC - fallback:[PASS] post_uses_wide_layout=1≥1;[PASS] post_width_is_scoped=1≥1;[PASS] desktop_tables_fill_post=1≥1;[PASS] toc_links_stay_white=1≥1;[PASS] toc_div_present=1≥1;[PASS] toc_builder_is_present=3≥2 |
| F-13 | B | browser | ✅ PASS | 0.6s | Syntax highlighting via hljs - fallback:[PASS] post_includes_hljs_script_tag=2≥1;[PASS] post_has_pre_code_blocks=10≥1 |
| F-14 | B | browser | ✅ PASS | 0.4s | Giscus comments - fallback:[PASS] post_includes_giscus_app_ref=1≥1 |
| F-15 | B | browser | ✅ PASS | 0.6s | Per-post meta tags - fallback:[PASS] post_has_og_image_meta=1≥1;[PASS] post_has_twitter_card_meta=1≥1 |
| F-16 | B | browser | ✅ PASS | 0.4s | Lazy + async on every markdown <img> - fallback:[PASS] post_imgs_lazy_async_pair=1≥1 |
| F-17 | B | browser | ✅ PASS | 0.6s | Reduced-motion guard - fallback:[PASS] css_has_prefers_reduced_motion=1≥1;[PASS] css_disables_animation_under_rm=2≥1 |
| F-18 | B | browser | ✅ PASS | 0.9s | Visible keyboard focus rings - fallback:[PASS] css_uses_focus_visible_pseudo=11≥1;[PASS] css_focus_visible_has_outline=2≥1;[PASS] css_focus_visible_input_outline=1≥1 |
| F-19 | B | browser | ✅ PASS | 0.6s | Skip-to-content link - fallback:[PASS] home_has_skip_anchor=1≥1;[PASS] home_has_main_target=1≥1 |
| O-01 | C | skip | ⏭️ SKIP | 0.0s | Docker build ×copy fixed in BG-01; O-01 follow-up still HIGH (BG-14) (skipped) |
| O-02 | C | build | ✅ PASS | 0.0s | exit 0 - nginx compression |
| O-04 | C | build | ✅ PASS | 0.0s | exit 0 - Makefile targets |
| O-05 | C | build | ✅ PASS | 0.0s | exit 0 - CI workflow triggers and tests |
| O-06 | C | build | ✅ PASS | 0.0s | exit 0 - robots.txt valid directives |
| O-08 | C | build | ✅ PASS | 0.0s | exit 0 - README consistency |

_Summary:_ total=38 pass=34 fail=0 skip=4
