</main>

<hr/>

<center style="text-decoration: none">

[`feed`](https://sanixdk.xyz/feed.xml)  [`github`](https://github.com/sanix-darker "Where i sleep in term of projects.")  [`telegram`](https://t.me/sanixdarker "You can Conctact me here anytime.")  [`email`](mailto:s4nixdk@gmail.com?subject=Hello%20there "You can email me anytime.")

</center>
</div>
<br/>

<script>
(() => {
    const searchInput = document.getElementById('search-input');
    if (!searchInput) return;

    const searchCount = document.getElementById('search-count');
    const blogItems = Array.from(document.querySelectorAll('.blog-item'));
    const blogList = document.querySelector('.blog-list');
    const haystacks = blogItems.map(item => {
        const title = item.querySelector('.blog-title');
        const tags = Array.from(item.querySelectorAll('.tag'))
            .map(tag => tag.textContent);
        return [title ? title.textContent : '', ...tags]
            .join(' ')
            .toLowerCase();
    });

    let noResults = document.getElementById('no-results');
    if (!noResults && blogList) {
        noResults = document.createElement('div');
        noResults.id = 'no-results';
        noResults.className = 'no-results is-hidden';
        noResults.innerHTML = '<p>// no posts found matching your search</p>';
        blogList.appendChild(noResults);
    }

    const render = () => {
        const term = searchInput.value.trim().toLowerCase();
        let visible = 0;

        blogItems.forEach((item, index) => {
            const matches = term === '' || haystacks[index].includes(term);
            item.classList.toggle('is-hidden', !matches);
            if (matches) visible++;
        });

        if (searchCount) {
            searchCount.textContent = term === ''
                ? `${blogItems.length} posts`
                : `${visible} / ${blogItems.length} match`;
        }
        if (noResults) {
            noResults.classList.toggle('is-hidden', visible !== 0 || term === '');
        }
    };

    searchInput.addEventListener('input', render);
    searchInput.addEventListener('keydown', event => {
        if (event.key === 'Escape') {
            searchInput.value = '';
            render();
        }
    });

    if (window.matchMedia('(min-width: 769px)').matches) {
        searchInput.focus();
    }
    render();
})();
</script>
</body>
</html>
