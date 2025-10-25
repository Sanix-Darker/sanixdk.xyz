<div id="index">
<div class="terminal-section">
    <div class="command-line">
        <span class="prompt">root@sanixdk:~$</span>
        <span class="command">whoami</span>
    </div>
    <div class="output">
        <span class="status-indicator status-online"></span>Backend Engineer, Electronics Geek & Otaku<br>
        <span class="status-indicator status-busy"></span>Currently solving interesting problems and exploring tech
    </div>
</div>

<h1 class='typing'> HI, AM SANIX </h1>

<div class="terminal-section skills-section">
    <div class="command-line">
        <span class="prompt">root@sanixdk:~$</span>
        <span class="command">tail -f /var/log/activity.log</span>
    </div>
    <div class="output">
        <div class="skills-grid">
            <div class="skill-item">Python</div>
            <div class="skill-item">JavaScript</div>
            <div class="skill-item">Go</div>
            <div class="skill-item">Rust</div>
            <div class="skill-item">Docker</div>
            <div class="skill-item">Linux</div>
            <div class="skill-item">Git</div>
            <div class="skill-item">Electronics</div>
            <div class="skill-item">Arduino</div>
            <div class="skill-item">Raspberry Pi</div>
            <div class="skill-item">IoT</div>
            <div class="skill-item">...</div>
        </div>
    </div>
</div>

<div class="terminal-section github-stats">
    <div class="command-line">
        <span class="prompt">root@sanixdk:~$</span>
        <span class="command">curl $GITHUB_HOST/sanixdk</span>
    </div>
    <div class="output">
        Public Repositories: 232<br>
        Most Used Languages: Python, JavaScript, Go, Shell<br>
        <div class="progress-bar">
            <div class="progress-fill" style="width: 92%;"></div>
        </div>
    </div>
</div>

<h2>WORKING ON:</h2>

<div class="terminal-section electronics-section">
    <div class="command-line">
        <span class="prompt">root@sanixdk:~$</span>
        <span class="command">ls -lah ~/github/*</span>
    </div>
    <div class="output">
        drwxr-xr-x 2 sanix 4096 Nov 15 2025 ./gitMark/<br>
        drwxr-xr-x 2 sanix 4096 Nov 10 2025 ./stackfetch/<br>
        drwxr-xr-x 2 sanix 4096 Nov 05 2025 ./zzollo/<br>
        drwxr-xr-x 2 sanix 4096 Dec 28 2024 ./gitrows-api/<br>
    </div>
</div>

<h2>CONTRIBUTING TO:</h2>
<ul>
    <li><a href="https://github.com/mendableai/firecrawl">firecrawl</a> : Turn entire websites into LLM-ready content. Scrape/Crawl/Extract with API.</li>
    <li><a href="https://github.com/osscameroon">oss-cammeroon</a> :  Open source projects driven by Cameroonian developers.</li>
    <li><a href="https://github.com/oven-sh/bun">bun</a> : fast JavaScript runtime, bundler, test runner, and package manage.</li>
    <li><a href="https://github.com/junegunn/fzf.vim">fzf-vim</a> : fzf in (N)vim.</li>
    <li><a href="https://github.com/dlvhdr/gh-dash">gh-dash</a> : A TUI dashboard for PR and issues accross github.</li>
    <li><a href="https://github.com/ln-dev7/sharuco">Sharuco</a> : OpenSource platform for sharing code/manage Forms (like google form).</li>
</ul>

<div class="interactive-terminal">
    <div class="command-line">
        <span class="prompt">you@sanixdk:~$</span>
        <input type="text" class="terminal-input" id="terminalInput" placeholder="type 'help' for commands" autocomplete="off">
    </div>
    <div id="terminalOutput"></div>
</div>

<script>
    // Interactive Terminal
    const terminalInput = document.getElementById('terminalInput');
    const terminalOutput = document.getElementById('terminalOutput');

    const commands = {
        help: 'Available commands: help, about, skills, projects, electronics, contact, clear, whoami, uptime',
        about: 'Backend Engineer passionate about solving interesting problems, building tools, and exploring electronics.',
        skills: 'Python, JavaScript, Go, Rust, Docker, Linux, Neovim, Git, Electronics, Arduino, Raspberry Pi, IoT',
        projects: 'gitMark, stackfetch, snips.nvim, dockdb.nvim, zzollo, gitrows-api, tchaka, oCloud/oBot, s2c',
        electronics: 'Arduino projects, Raspberry Pi setups, IoT sensors, PCB designs, circuit prototyping',
        contact: 'GitHub: github.com/sanix-darker | Telegram: @sanixdarker | Email: s4nixdk@gmail.com',
        clear: '',
        whoami: 'sanix - Backend Engineer, Electronics Geek & Otaku',
        uptime: 'System uptime: Always learning, always building'
    };

    if (terminalInput) {
        terminalInput.addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                const command = this.value.trim().toLowerCase();
                const output = document.createElement('div');
                output.style.marginBottom = '10px';
                output.style.color = 'var(--text-secondary)';

                if (command === 'clear') {
                    terminalOutput.innerHTML = '';
                } else if (commands[command]) {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>${commands[command]}`;
                    terminalOutput.appendChild(output);
                } else if (command) {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br><span style="color: var(--text-muted);">command not found: ${command}</span>`;
                    terminalOutput.appendChild(output);
                }

                this.value = '';
                terminalOutput.scrollTop = terminalOutput.scrollHeight;
            }
        });
    }

    // Real GitHub API calls and dynamic data
    async function fetchGitHubData() {
        try {
            // Fetch user data
            const userResponse = await fetch('https://api.github.com/users/sanix-darker');
            const userData = await userResponse.json();

            // Update public repos count
            const reposElement = document.querySelector('.github-stats .output');
            if (reposElement && userData.public_repos) {
                reposElement.innerHTML = `
                    Public Repositories: ${userData.public_repos}<br>
                    Followers: ${userData.followers} | Following: ${userData.following}<br>
                    <div class="progress-bar">
                        <div class="progress-fill" style="width: ${Math.min(userData.public_repos, 100)}%;"></div>
                    </div>
                `;
            }

            // Fetch recent repositories for electronics section
            const reposResponse = await fetch('https://api.github.com/users/sanix-darker/repos?sort=updated&per_page=15');
            const reposData = await reposResponse.json();

            // Update electronics section with recent repos
            const electronicsElement = document.querySelector('.electronics-section .output');
            if (electronicsElement && reposData.length > 0) {
                let reposList = '';
                reposData.forEach(repo => {
                    const updatedDate = new Date(repo.updated_at).toLocaleDateString('en-US', {
                        month: 'short',
                        day: '2-digit'
                    });
                    const language = repo.language || 'N/A';
                    reposList += `drwxr-xr-x ${language} dk 1 ${updatedDate} ./${repo.name}/<br>`;
                });
                electronicsElement.innerHTML = reposList;
            }

        } catch (error) {
            console.error('Error fetching GitHub data:', error);
        }
    }

    // Fetch WakaTime stats from profile README
    async function fetchWakaTimeStats() {
        try {
            const response = await fetch('https://raw.githubusercontent.com/Sanix-Darker/sanix-darker/refs/heads/master/README.md');
            const readmeText = await response.text();

            // Extract WakaTime section
            const wakaStart = readmeText.indexOf('<!--START_SECTION:waka-->');
            const wakaEnd = readmeText.indexOf('<!--END_SECTION:waka-->');

            if (wakaStart !== -1 && wakaEnd !== -1) {
                const wakaSection = readmeText.substring(wakaStart, wakaEnd);

                // Parse coding time
                const codingTimeMatch = wakaSection.match(/Coding time : (.+?)\./);
                const codingTime = codingTimeMatch ? codingTimeMatch[1] : '1 hr 20 mins';

                // Parse languages with percentages
                const languageMatches = wakaSection.matchAll(/(\w+)\s+(\d+\s+\w+)\s+.*?(\d+\.\d+)\s*%/g);
                const languages = Array.from(languageMatches).slice(0, 6); // Top 6 languages

                // Update skills section
                const skillsElement = document.querySelector('.skills-section .output');
                if (skillsElement && languages.length > 0) {
                    let skillsGrid = '<div class="skills-grid">';

                    // Add dynamic languages from WakaTime
                    languages.forEach(([, lang, time, percentage]) => {
                        skillsGrid += `<div class="skill-item">${lang} (${percentage}%)</div>`;
                    });

                    // Add static skills
                    const staticSkills = [];
                    staticSkills.forEach(skill => {
                        skillsGrid += `<div class="skill-item">${skill}</div>`;
                    });

                    skillsGrid += '</div>';
                    skillsGrid += `<br>Weekly Coding Time: ${codingTime}`;

                    skillsElement.innerHTML = skillsGrid;
                }
            }

        } catch (error) {
            console.error('Error fetching WakaTime stats:', error);
        }
    }

    // Enhanced terminal commands with real data
    const enhancedCommands = {
        ...commands,
        repos: 'Fetching latest repositories...',
        stats: 'Fetching GitHub statistics...',
        waka: 'Fetching WakaTime coding stats...',
        refresh: 'Refreshing all data...'
    };

    // Update terminal input handler
    if (terminalInput) {
        terminalInput.removeEventListener('keypress', terminalInput.keypressHandler);

        terminalInput.keypressHandler = async function(e) {
            if (e.key === 'Enter') {
                const command = this.value.trim().toLowerCase();
                const output = document.createElement('div');
                output.style.marginBottom = '10px';
                output.style.color = 'var(--text-secondary)';

                if (command === 'clear') {
                    terminalOutput.innerHTML = '';
                } else if (command === 'repos') {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>Fetching latest repositories...`;
                    terminalOutput.appendChild(output);
                    await fetchGitHubData();
                } else if (command === 'stats') {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>Fetching GitHub statistics...`;
                    terminalOutput.appendChild(output);
                    await fetchGitHubData();
                } else if (command === 'waka') {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>Fetching WakaTime coding stats...`;
                    terminalOutput.appendChild(output);
                    await fetchWakaTimeStats();
                } else if (command === 'refresh') {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>Refreshing all data...`;
                    terminalOutput.appendChild(output);
                    await Promise.all([fetchGitHubData(), fetchWakaTimeStats()]);
                } else if (enhancedCommands[command]) {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br>${enhancedCommands[command]}`;
                    terminalOutput.appendChild(output);
                } else if (command) {
                    output.innerHTML = `<span style="color: var(--text-muted);">you@sanixdk:~$</span> ${this.value}<br><span style="color: var(--text-muted);">command not found: ${command}</span>`;
                    terminalOutput.appendChild(output);
                }

                this.value = '';
                terminalOutput.scrollTop = terminalOutput.scrollHeight;
            }
        };

        terminalInput.addEventListener('keypress', terminalInput.keypressHandler);
    }

    // Load data on page load
    document.addEventListener('DOMContentLoaded', function() {
        // Add loading indicators
        const githubStats = document.querySelector('.github-stats .output');
        const skillsSection = document.querySelector('.skills-section .output');
        const electronicsSection = document.querySelector('.electronics-section .output');

        if (githubStats) githubStats.innerHTML = 'Loading GitHub data...';
        if (skillsSection) skillsSection.innerHTML = 'Loading skills data...';
        if (electronicsSection) electronicsSection.innerHTML = 'Loading recent repositories...';

        // Fetch all data
        Promise.all([fetchGitHubData(), fetchWakaTimeStats()]).then(() => {
            console.log('All data loaded successfully');
        }).catch(error => {
            console.error('Error loading data:', error);
        });
    });
</script>
</div>
