---
map: 1
scope: lib
parent: ../MAP.md
fidelity: syntax
api_hash: 0924c38d8a108686
tokens: ~327
stamped: 2026-07-16T20:18:04Z
---
# lib

<!-- radar:slot purpose max=160 -->
Implements Markdown rendering, metadata and RSS output, filesystem traversal, HTML entities, and the bundled C parser.
<!-- /radar:slot -->

## API
`entity.c`
- `const struct entity* entity_lookup(const char* name, size_t name_size)`
`lib.c`
- `void buildComponentsIntoMarkdownsFiles(const char* directory)`
- `void minifyDirfiles(const char* path)`
- `void writeMetadatasToBlogList(const char* input_filename,`
- `void proceedFilesRecursively(char* basePath)`
`md4c-html.c`
- `int md_html(const MD_CHAR* input, MD_SIZE input_size,`
- also: Entry, EntryMap, MD_ALIGN, MD_ATTRIBUTE, MD_ATTRIBUTE_BUILD, MD_ATTRIBUTE_BUILD_tag, MD_BLOCK, MD_BLOCKTYPE, MD_BLOCK_CODE_DETAIL, MD_BLOCK_H_DETAIL, MD_BLOCK_LI_DETAIL, MD_BLOCK_OL_DETAIL, MD_BLOCK_TABLE_DETAIL, MD_BLOCK_TD_DETAIL, MD_BLOCK_UL_DETAIL, MD_BLOCK_tag, MD_CHAR, MD_CONTAINER, MD_CONTAINER_tag, MD_CTX, MD_CTX_tag, MD_HTML, MD_HTML_tag, MD_LINE, MD_LINETYPE, MD_LINETYPE_tag
- +57 more public symbols omitted by the map budget

## Jump
- `buildComponentsIntoMarkdownsFiles` ← used by main.c
