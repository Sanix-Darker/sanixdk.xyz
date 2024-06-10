![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-sign-your-commits/preview.jpg)

### HOW TO SIGN YOUR COMMITS ON GITHUB OR GITLAB
`2024-06-15 11:12AM` • 5 min read • **#git** **#security** **#gpg**

This tutorial will guide you through creating a GPG key, adding it to your GitHub or GitLab account, and using it to sign your commits. Signing commits helps ensure the authenticity and integrity of your code by verifying that the commits were indeed made by you.

#### WHAT IS A GPG KEY?

A GPG (GNU Privacy Guard) key is a cryptographic key used for securing communication and data. It is part of the GPG encryption tool, which provides cryptographic privacy and authentication through the use of public and private key pairs. In the context of Git, a GPG key allows you to sign your commits and tags, proving that they were made by you and have not been tampered with.

#### CREATE YOUR GPG KEY

First, we will create a GPG key and add it to GitHub or GitLab.

##### Step 1: Generate a GPG Key

1. **Check if a GPG key already exists and generate one if it doesn't:**

    ```bash
    # Check if a GPG key already exists
    GPG_KEY=$(gpg --list-secret-keys --keyid-format LONG | awk '/^sec/ { getline; print $1 }')
    if [ -z "$GPG_KEY" ]; then
        echo "No GPG key found. Generating a new GPG key..."
        # Generate a new GPG key
        gpg --full-generate-key
    fi
    ```

    This command checks if a GPG key exists. If not, it generates a new GPG key.

2. **Print the generated public key:**

    ```bash
    # Fetch the generated GPG key
    GPG_KEY=$(gpg --list-secret-keys --keyid-format LONG | awk '/^sec/ { getline; print $1 }')
    echo "Generated public key for $GPG_KEY:"
    gpg --armor --export "$GPG_KEY"
    ```

    This command prints the generated public key.

3. **Follow the prompts to generate your GPG key:**

    - Select the type of key you want (e.g., RSA and RSA).
    - Choose the key size (2048 or 4096 bits).
    - Set the expiration date for the key.
    - Provide your name and email address.
    - Set a passphrase for the key.

##### Step 2: Add Your GPG Key to GitHub or GitLab

1. **Copy your GPG key:**

    After running the commands above, the public key will be printed. Copy the output starting from `-----BEGIN PGP PUBLIC KEY BLOCK-----` to `-----END PGP PUBLIC KEY BLOCK-----`.

2. **Add GPG Key to GitHub:**

    - Go to your [GitHub GPG keys settings](https://github.com/settings/keys).
    - Click "New GPG key".
    - Paste your GPG key into the provided field and click "Add GPG key".

3. **Add GPG Key to GitLab:**

    - Go to your [GitLab GPG keys settings](https://gitlab.com/profile/gpg_keys).
    - Click "Add new GPG key".
    - Paste your GPG key into the provided field and click "Add key".

#### SIGNING YOUR COMMITS

##### Step 3: Configure Git to Sign Your Commits

1. **Set your GPG key in Git:**

    ```bash
    git config --global user.signingkey <YOUR_GPG_KEY_ID>
    ```

    Replace `<YOUR_GPG_KEY_ID>` with your GPG key ID. You can find this in the output of `gpg --list-secret-keys --keyid-format LONG`.

2. **Tell Git to sign all your commits by default:**

    ```bash
    git config --global commit.gpgSign true
    ```

3. **Optionally, sign individual commits:**

    If you prefer to sign commits individually, use the `-S` flag:

    ```bash
    git commit -S -m "bloops bloops bloops"
    ```

##### Step 4: Verify Signed Commits on GitHub or GitLab

1. **Push your signed commits to GitHub or GitLab:**

    ```bash
    git push origin main
    ```

2. **Check the commits in the repository:**

    On GitHub or GitLab, navigate to your repository and look at your commit history. Signed commits will have a `Verified` badge next to them.

Congratulations! You've successfully generated a GPG key, added it to your GitHub or GitLab account, and configured Git to sign your commits. This enhances the security and authenticity of your commits by verifying that they were indeed made by you.

-----------
