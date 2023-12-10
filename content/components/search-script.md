<script type="text/javascript" >
    document.getElementById("inp").addEventListener(
        "keyup",
        () => {
            var items = document.getElementsByTagName("h3");
            var hide = true;

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
    );
    const countBlogs = document.querySelectorAll("h3").length;
    document.getElementById("blog-count").innerHTML = `(${countBlogs})`
</script>
