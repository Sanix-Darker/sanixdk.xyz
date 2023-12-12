<script type="text/javascript" >
    var items = document.getElementsByTagName("h3");
    var hide = true;

    const filter = () => {
        for (var i = 0; i < items.length; i++) {
            var item = items[i];

            // NOTE: this code is ugly... and i like it. XD
            if (
                item.innerHTML.toLowerCase().indexOf(
                    document.getElementById("inp").value.toLowerCase()
                ) > -1
            ) {
              item.style.display = "block";
              item.style.position = "relative";
              hide = false;
            } else if (hide) {
              item.style.display = "none";
              item.style.position = "absolute";
            }
        }
    }
    window.addEventListener('load', function() {
        var queryParams = new URLSearchParams(window.location.search);
        var query = queryParams.get('q');
        var element = document.getElementById('inp');

        if (typeof query !== "undefined" && query !== null && query.length > 0){
          element.value = query;
          filter();
        }
    });

    document.getElementById("inp").addEventListener(
        "keyup", filter
    );
    // count the amount of blog-posts
    const countBlogs = document.querySelectorAll("h3").length;
    document.getElementById("blog-count").innerHTML = `(${countBlogs})`
</script>
