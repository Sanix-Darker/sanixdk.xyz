[<< blogs](/blogs/)

<script src="https://giscus.app/client.js"
        data-repo="sanix-darker/sanixdk.xyz"
        data-repo-id="R_kgDOJSF_ow"
        data-category="General"
        data-category-id="DIC_kwDOJSF_o84CXz0s"
        data-mapping="pathname"
        data-strict="0"
        data-reactions-enabled="1"
        data-emit-metadata="0"
        data-input-position="top"
        data-theme="dark"
        data-lang="en"
        data-loading="lazy"
        crossorigin="anonymous"
        async>
</script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
<script>hljs.highlightAll(); </script>
<script>
// To handle TOC
(function() {
    const tocContainer = document.getElementById('toc-container');

    if(!tocContainer) {
        return;
    }
    const headingSelectors = ['h3', 'h4', 'h5', 'h6'];
    const headings = Array.from(document.querySelectorAll(headingSelectors.join(', ')));

    let tocHtml = '<ul>';
    let currentLevel = 3;

    headings.forEach((heading, index) => {
        if (!heading.id) {
            heading.id = 'toc-heading-' + index;
        }
        const level = parseInt(heading.tagName.substring(1));

        while (level > currentLevel) {
            tocHtml += '<ul>';
            currentLevel++;
        }

        while (level < currentLevel) {
            tocHtml += '</ul></li>';
            currentLevel--;
        }

        tocHtml += `<li><a href="#${heading.id}">${heading.textContent}</a>`;
    });

    while (currentLevel > 2) {
        tocHtml += '</ul></li>';
        currentLevel--;
    }

    tocHtml += '</ul>';
    tocContainer.innerHTML += tocHtml;
})();
</script>
