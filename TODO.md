# We can inspire ourself from https://github.com/dominickpastore/pymd4c
# we need to parse the markdown with md4c
# then build the html element

```python
import md4c
with open('README.md', 'r') as f:
    markdown = f.read()
renderer = md4c.HTMLRenderer()
html = renderer.parse(markdown)
```

- setUp the CI/CD
    - add a CI/CD to publish the website when there is an update on the personnal website path
    - add a CI/CD to build and deploy my blog contents (for each blog change detected)
    - add a CI/CD to publish on all my social network a message for updates on my blog or projects.

- track visits on each blog page

- Creer un volume pour partager les fichiers build avec l'exterieur du container
    Ceci permet d'eviter un nieme forwarding de requettes a l'interieur du container avec nginx...
    Constat fait, cela ralentit les requettes...
