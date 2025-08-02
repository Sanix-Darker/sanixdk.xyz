

<hr/>

<center>

[`github`](https://github.com/sanix-darker "Where i sleep in term of projects.")
◦ [`telegram`](https://t.me/sanixdarker "You can Conctact me here anytime.")
◦ [`email`](mailto:s4nixdk@gmail.com?subject=Hello%20there "You can email me anytime.")

</center>
            </div>
        <br/>
        <script>
            document.addEventListener('DOMContentLoaded', function() {
                document.getElementById("blog-list").style.setProperty("--total-items", `${document.querySelectorAll("h3").length}`);
                const searchInput = document.getElementById('search-input');
                const blogItems = document.querySelectorAll('.blog-item, article');

                if (!searchInput) return;

                searchInput.addEventListener('input', function() {
                    const searchTerm = this.value.toLowerCase().trim();

                    blogItems.forEach(function(item) {
                        const title = item.querySelector('h1, h2, h3, .blog-title');
                        const content = item.querySelector('p, .blog-excerpt, .content');
                        const tags = item.querySelectorAll('.tag, .tags span');

                        let searchableText = '';

                        // Add title text
                        if (title) {
                            searchableText += title.textContent.toLowerCase() + ' ';
                        }

                        // Add content text
                        if (content) {
                            searchableText += content.textContent.toLowerCase() + ' ';
                        }

                        // Add tags text
                        tags.forEach(function(tag) {
                            searchableText += tag.textContent.toLowerCase() + ' ';
                        });

                        // Show/hide based on search
                        if (searchTerm === '' || searchableText.includes(searchTerm)) {
                            item.style.display = 'block';
                            item.style.opacity = '1';
                        } else {
                            item.style.display = 'none';
                            item.style.opacity = '0';
                        }
                    });

                    // Show "no results" message if needed
                    const visibleItems = Array.from(blogItems).filter(item =>
                        item.style.display !== 'none'
                    );

                    let noResultsMsg = document.getElementById('no-results');
                    if (visibleItems.length === 0 && searchTerm !== '') {
                        if (!noResultsMsg) {
                            noResultsMsg = document.createElement('div');
                            noResultsMsg.id = 'no-results';
                            noResultsMsg.className = 'no-results';
                            noResultsMsg.innerHTML = '<p>// no posts found matching your search</p>';

                            const blogContainer = document.querySelector('.blog-list, .blog-posts, main');
                            if (blogContainer) {
                                blogContainer.appendChild(noResultsMsg);
                            }
                        }
                        noResultsMsg.style.display = 'block';
                    } else if (noResultsMsg) {
                        noResultsMsg.style.display = 'none';
                    }
                });
            });
        </script>
    </body>
</html>
