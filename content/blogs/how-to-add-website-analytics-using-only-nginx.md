<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-add-website-analytics-using-only-nginx/akii.png" />

## HOW TO TRACK WEBSITE ANALYTICS USING ONLY NGINX AND PURE BASH

`2025-01-26 10:04` • 19 min read • **#bash** **#nginx** **#terminal**

---

### DISCLAIMER AS INTRO


Hi there, what's up ?

Today, am going to share with you a 'small' tip, using only `bash` and `nginx` to have google-analytics's like, tracking metrics service on your websites,

You may be wondering... *"WHY DOING THAT ?"*; you see, long time ago, i decided to make my personal website using only C.

Yup, the one you're looking right now, and reading from; and i used a lot of existing "markdown -> html" C code to add my own sauce (it's a little messy but i like it).

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-add-website-analytics-using-only-nginx/how-to-add-website-analytics-using-only-nginxccc.png)

[SEE THE REPO](https://github.com/Sanix-Darker/sanixdk.xyz)

This was a really really really BAD idea....
did i said "really" enough ?

Why ? well... basically, most of my blogs are markdowns, to match the 'modern way of browsing the internet' i made a `lib.c` that rule the website build (am not going to go deeper on this article tho).

But hey, making mistakes makes you grow... right ?.... RIGHHHT ?

Anyway, the problem with building something html/markdown only is that you don't have JS involves... and tbh, that was the first point (not because i hate modern JS *heavy :wink: to my JS homies*);

Therefore, it was a 'NO GO on google analytics' for me...

Was it worth it? *ABSOLUTELLY NO*, will i do that again, *OH, HELL YEAH*!

ANYWAY... let's jump into it, shall we ?

---

### THE SERIOUS INTRO

Since i was just returning basic html, after a build of the website/blogs, no "serious JS (except the github comments at the bottom of blogs)" involves, there were no way for me to track who, how much visited this page, or that page.

BUT, i was(still) using nginx for the requests handling part and i asked myself, if that service logs basically access-logs, why not use it to collect metrics directly ? (fair enough right ?).

So, adding an "analytics system" based on Nginx typically involves configuring Nginx to basically just log requests (with enough of verbose infos), process those logs to count views, ips, timing and possibly visualize the data.

---

### HOW TO 'ACTCHUALLY' DO THAT

#### ENABLE LOGGING IN NGINX

You know, nginx logs requests by default, but you may need to customize the log format to track relevant details, such as URLs or IP addresses.

Edit your Nginx configuration file, typically located at `/etc/nginx/nginx.conf` or `/etc/nginx/sites-available/<your-site>`.

```bash
$ vim /etc/nginx/nginx.conf
# or 'nano' as the editor, if you're a noob like me.
```

then in the `http` section,

```ini
# Simple example for views
http {
    log_format views '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$host" "$request_uri"';

    access_log /var/log/nginx/access.log views;
}
```

- **`$remote_addr`**: Tracks the client IP.
- **`$host`** and **`$request_uri`**: Capture the domain and URL.

reload Nginx to apply changes:

```bash
$ sudo systemctl reload nginx
```

Optionally, you can also check if your nginx config is still good or not with :

```bash
$ nginx -t
```

Below is an example that captures much more in terms of tracking :

```ini
# Most verbose example with more infos than before
http {
    log_format analytics '$remote_addr - $remote_user [$time_local] '
                        '"$request" $status $body_bytes_sent '
                        '"$http_referer" "$http_user_agent" '
                        '$request_time $upstream_response_time '
                        '$scheme $server_name $request_method '
                        '$ssl_protocol $ssl_cipher';

    access_log /var/log/nginx/access.log analytics;
}
```

1. **IP Address**: `$remote_addr`
2. **Username** (if Basic Auth): `$remote_user`
3. **Timestamp**: `$time_local`
4. **Raw Request**: `$request` (includes URL, query, HTTP version)
5. **HTTP Status**: `$status`
6. **Response Size**: `$body_bytes_sent`
7. **Referrer**: `$http_referer`
8. **User Agent**: `$http_user_agent`
9. **Request Time**: `$request_time`
10. **Upstream Time**: `$upstream_response_time`
11. **Scheme**: `$scheme` (http/https)
12. **Server Name**: `$server_name`
13. **Request Method**: `$request_method` (GET/POST, etc.)
14. **SSL Protocol**: `$ssl_protocol` (e.g., TLSv1.3)
15. **SSL Cipher**: `$ssl_cipher` (e.g., AES128-GCM-SHA256)


And "VOILA", the most important part is done, you now have all requests logs going to the path specified in your config,
in our case, */var/log/nginx/access.log*, it will looks like :

```bash
$ cat /var/log/nginx/access.log
```

```txt
18.220.122.122 - - [12/Jan/2025:17:15:19 +0000] "GET / HTTP/1.1" 400 666 "-" "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) Chrome/126.0.0.0 Safari/537.36" "147.182.205.116" "/"$

54.36.148.9 - - [12/Jan/2025:17:16:15 +0000] "GET /robots.txt HTTP/1.1" 200 719 "-" "Mozilla/5.0 (compatible; AhrefsBot/7.0; +http://ahrefs.com/robot/)" "sanixdk.xyz" "/robots.txt"$

51.222.253.18 - - [12/Jan/2025:17:16:16 +0000] "GET /blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3 HTTP/1.1" 200 7569 "-" "Mozilla/5.0 (compatible; AhrefsBot/7.0; +http://ahrefs.com/robot/)" "sanixdk.xyz" "/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3"$

124.44.90.81 - - [12/Jan/2025:17:22:41 +0000] "GET / HTTP/1.0" 404 162 "-" "curl/7.88.1" "147.182.205.116" "/"$

95.214.55.226 - - [12/Jan/2025:17:26:19 +0000] "GET / HTTP/1.1" 404 162 "-" "-" "147.182.205.116" "/"$
```

Now, we have those, let's read, parse, understand and exploit them.

---

### NOW, LET's DO SOME PROGRAMMING

You know i like bash...
 I mean, i LOVE bash,
 WHY ?

Well, it should be obvious by now, but, whatever linux distro you got into... Or you just installed, it's already there, no need to install `gcc`, or `python` or anything...

Because it's the shell btw, so we're going to use it.

Let's break down each steps, from the source code to the service collecting all the metrics we need.

---

#### THE SOURCE CODE:

Each instruction is documented as much as possible, fill free to ask questions in the comment section.

```bash
#!/usr/bin/env bash
#
# nginx-analytics.sh - Collect Nginx log metrics and output JSON analytics.
#
# by dk (https://github.com/sanix-darker)

LOG_FILE="/var/log/nginx/access.log"
OUTPUT_FILE="/var/www/html/nginx-analytics.json"

# Just a Safety check: Ensure LOG_FILE exists
if [[ ! -f "$LOG_FILE" ]]; then
  echo "Error: Nginx log file not found at $LOG_FILE"
  exit 1
fi

# 1) Visits by URL (Top 5)
#    - Using 'awk' to extract the URL from the "$request" field (assuming it's at $5).
#    - Then split the request string "METHOD /some/url HTTP/1.x" into tokens,
#      and keep the second token (the actual path /some/url).
visits_by_url=$(
  awk '{
    # $5 might be something like: "GET /index.html HTTP/1.1"
    # So we split on space.
    split($5, req_parts, " ")
    url=req_parts[2]
    if (url != "") {
      urls[url]++
    }
  } END {
    # Sort by frequency and pick top 5
    for (u in urls) {
      print urls[u] " " u
    }
  }' "$LOG_FILE" \
  | sort -nr \
  | head -n 5
)

# 2) Top IP addresses (Top 5)
top_ips=$(
  awk '{
    ips[$1]++
  } END {
    for (ip in ips) {
      print ips[ip] " " ip
    }
  }' "$LOG_FILE" \
  | sort -nr \
  | head -n 5
)

# 3) Average request time ($request_time), assume it's field #10 in our log format
avg_request_time=$(
  awk '{
    sum+=$10; count++
  } END {
    if (count > 0) {
      printf("%.5f", sum/count)
    } else {
      print "0"
    }
  }' "$LOG_FILE"
)

# 4) Average upstream response time ($upstream_response_time), assume it's field #11
avg_upstream_time=$(
  awk '{
    sum+=$11; count++
  } END {
    if (count > 0) {
      printf("%.5f", sum/count)
    } else {
      print "0"
    }
  }' "$LOG_FILE"
)

# 5) Top HTTP status codes (Top 5), assume it's field #6
top_status_codes=$(
  awk '{
    status[$6]++
  } END {
    for (s in status) {
      print status[s] " " s
    }
  }' "$LOG_FILE" \
  | sort -nr \
  | head -n 5
)

# ------------------------------------------------------------
# Now, the less painfull section,
# Convert the above variables to a comprehensible JSON.
# We'll do minimal parsing to produce valid JSON arrays/objects.
# ------------------------------------------------------------

# 'lines_to_json_array' to convert "count value" lines into JSON array entries
# e.g., "123 /home" -> { "value": "/home", "count": 123 }
function lines_to_json_array() {
  local input="$1"
  local result="["
  local first=1
  while IFS= read -r line; do
    # Here we just split line into "count" and "value"
    count=$(echo "$line" | awk '{print $1}')
    value=$(echo "$line" | awk '{print $2}')
    # IMPORTANT NOTE:
    # If "value" has spaces, handle carefully. (In simplest cases, it won't.)
    # If you have more complex parsing, you'd do more robust splitting.
    if [ "$first" -eq 1 ]; then
      first=0
    else
      result="$result,"
    fi
    result="$result {\"value\":\"$value\",\"count\":$count}"
  done <<< "$input"
  result="$result]"
  echo "$result"
}

# visits_by_url -> JSON-array
visits_by_url_json=$(lines_to_json_array "$visits_by_url")

# top_ips -> JSON-array
top_ips_json=$(lines_to_json_array "$top_ips")

# top_status_codes -> JSON-array
top_status_codes_json=$(lines_to_json_array "$top_status_codes")

# ET VOILA, our json ready
json_output=$(
  cat <<EOF
{
  "visits_by_url": $visits_by_url_json,
  "top_ips": $top_ips_json,
  "avg_request_time": "$avg_request_time",
  "avg_upstream_time": "$avg_upstream_time",
  "top_status_codes": $top_status_codes_json
}
EOF
)

# Write JSON to OUTPUT_FILE (e.g., for a web dashboard or further processing)
echo "$json_output" > "$OUTPUT_FILE"

# Or simply echo to stdout if you prefer:
# echo "$json_output"

exit 0 # you can should to exit with something else, your code, you choose :wink:
```

Save the following to `/usr/local/bin/nginx-analytics.sh` (adjust paths as needed)

**NOTE:** Don't forget to make it executable

```bash
$ sudo chmod +x /usr/local/bin/nginx-analytics.sh
```

---

#### CREATE A SYSTEMD UNIT

Now we have our script, we need to make it run as a "service", so if the server boot, we still having it start in background, so below is an example **systemd service** file to run this script as a one-shot service.
Create a file `/etc/systemd/system/nginx-analytics.service`:

```ini
[Unit]
Description=Collect Nginx Log Metrics and Output JSON
After=network.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/nginx-analytics.sh

[Install]
WantedBy=multi-user.target
```

Now, *Reload systemd* to register the new service:
```bash
sudo systemctl daemon-reload
```

Then, *Run the service manually* (on demand):
```bash
sudo systemctl start nginx-analytics.service
```

And finally *Enable on boot* (if desired):
```bash
sudo systemctl enable nginx-analytics.service
```

---

#### (OPTIONAL... BUT I RECOMMEND ALOT) SET UP A SYSTEMD TIMER

If you want this script to run periodically (e.g., every 5 minutes or every hour, or every seconds for the crazy ones reading this), create a matching `.timer` unit. For example, `/etc/systemd/system/nginx-analytics.timer`:

```ini
[Unit]
Description=Run nginx-analytics service periodically

[Timer]
OnBootSec=5min
OnUnitActiveSec=1h

[Install]
WantedBy=timers.target
```

Then:
```bash
sudo systemctl daemon-reload
sudo systemctl enable --now nginx-analytics.timer
```
This will run `nginx-analytics.service` (and thus `nginx-analytics.sh`) once an hour, 5 minutes after boot.

---

#### WANNA SEE THE RESULTS ???

After each run, you’ll have a **`nginx-analytics.json`** file (e.g., at `/var/www/html/nginx-analytics.json`) containing a JSON object like:

  ```json
  {
    "visits_by_url": [
      {"value":"/index.html","count":125},
      {"value":"/contact","count":55},
      ...
    ],
    "top_ips": [
      {"value":"192.168.1.100","count":76},
      ...
    ],
    "avg_request_time": "0.03005",
    "avg_upstream_time": "0.01002",
    "top_status_codes": [
      {"value":"200","count":2000},
      {"value":"404","count":42},
      ...
    ]
  }
  ```

AND `VOILA` !

You can now consume or display this JSON in a web interface, push it to another system, or simply analyze it further with other tools.

For my use case i made a small html index view to consume it that looks like this :
```html

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>NGINX-ANALYTICS</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 20px; background: #f7f7f7; color: #333; } h1, h2, h3 { margin-top: 1em; margin-bottom: 0.5em; } .stats-container { display: flex; flex-wrap: wrap; gap: 2rem; } .stats-block { background: #fff; padding: 1rem; border-radius: 6px; box-shadow: 0 2px 5px rgba(0,0,0,0.15); flex: 1 1 350px; max-width: 500px; } table { width: 100%; border-collapse: collapse; margin-bottom: 1em; } th, td { text-align: left; padding: 8px; border-bottom: 1px solid #ddd; } th { background-color: #fafafa; } .bar-chart { position: relative; height: 20px; background: #eee; border-radius: 4px; overflow: hidden; } .bar { position: absolute; left: 0; top: 0; bottom: 0; background: #007acc; text-align: right; color: #fff; padding-right: 4px; font-size: 13px; line-height: 20px; } .numeric { font-weight: bold; color: #007acc; display: inline-block; margin-left: 0.3em; } /* Responsive */ @media (max-width: 600px) { .stats-container { flex-direction: column; } }
  </style>
</head>
<body>
  <h1>NGINX-ANALYTICS logs visualization</h1>
  <p>This page fetches <code>logs.json</code> and displays the metrics below.</p>
  <small>by Sanix-Darker</small>

  <div class="stats-container">
    <!-- Block for top URLs -->
    <div class="stats-block">
      <h2>Top URLs</h2>
      <table id="table-urls">
        <thead>
          <tr>
            <th>URL</th>
            <th>Count</th>
          </tr>
        </thead>
        <tbody></tbody>
      </table>
    </div>

    <!-- Block for top IP addresses -->
    <div class="stats-block">
      <h2>Top IPs</h2>
      <table id="table-ips">
        <thead>
          <tr>
            <th>IP Address</th>
            <th>Count</th>
          </tr>
        </thead>
        <tbody></tbody>
      </table>
    </div>

    <!-- Block for top HTTP status codes -->
    <div class="stats-block">
      <h2>Top Status Codes</h2>
      <table id="table-status-codes">
        <thead>
          <tr>
            <th>Status Code</th>
            <th>Count</th>
          </tr>
        </thead>
        <tbody></tbody>
      </table>
    </div>

    <!-- Block for average times -->
    <div class="stats-block">
      <h2>Timing</h2>
      <p>Average Request Time: <span id="avg-request-time" class="numeric">--</span></p>
      <p>Average Upstream Time: <span id="avg-upstream-time" class="numeric">--</span></p>
    </div>
  </div>

  <script>
    fetch('/logs.json')
      .then(response => {
        if (!response.ok) {
          throw new Error('Could not fetch logstats.json');
        }
        return response.json();
      })
      .then(data => {
        // data structure expected:
        // {
        //   "visits_by_url": [{ "value":"/index.html","count":125 }, ... ],
        //   "top_ips": [{ "value":"192.168.1.1","count":76 }, ... ],
        //   "avg_request_time": "0.03005",
        //   "avg_upstream_time": "0.01002",
        //   "top_status_codes": [{ "value":"200","count":2000 }, ... ]
        // }

        // 1) Populate Top URLs
        const urlTableBody = document.querySelector('#table-urls tbody');
        populateTable(urlTableBody, data.visits_by_url);

        // 2) Populate Top IPs
        const ipTableBody = document.querySelector('#table-ips tbody');
        populateTable(ipTableBody, data.top_ips);

        // 3) Populate Top Status Codes
        const statusTableBody = document.querySelector('#table-status-codes tbody');
        populateTable(statusTableBody, data.top_status_codes);

        // 4) Fill in average times
        document.getElementById('avg-request-time').textContent = data.avg_request_time;
        document.getElementById('avg-upstream-time').textContent = data.avg_upstream_time;
      })
      .catch(err => {
        console.error(err);
        document.body.innerHTML += '<p style="color:red;">Error fetching logstats.json</p>';
      });

    /**
     * populateTable: utility function to fill a table body given an array of
     * { value, count } objects, plus a quick "bar chart" approach.
     */
    function populateTable(tbody, items) {
      // Find the max count to scale bar widths
      let maxCount = 0;
      if (Array.isArray(items)) {
        items.forEach(item => {
          if (item.count > maxCount) {
            maxCount = item.count;
          }
        });
      }

      // Clear any existing rows
      tbody.innerHTML = '';

      if (!Array.isArray(items) || items.length === 0) {
        tbody.innerHTML = '<tr><td colspan="2">No data</td></tr>';
        return;
      }

      // Create rows
      items.forEach(item => {
        const row = document.createElement('tr');

        // 1st column (value)
        const tdValue = document.createElement('td');
        tdValue.textContent = item.value;
        row.appendChild(tdValue);

        // 2nd column (count + mini bar chart)
        const tdCount = document.createElement('td');

        // Container for the bar
        const barWrapper = document.createElement('div');
        barWrapper.className = 'bar-chart';

        // Actual bar
        const bar = document.createElement('div');
        bar.className = 'bar';
        const percentage = (item.count / maxCount) * 100;
        bar.style.width = percentage.toFixed(2) + '%';
        bar.textContent = item.count; // or you can keep it blank or put item.count at the right

        barWrapper.appendChild(bar);
        tdCount.appendChild(barWrapper);
        row.appendChild(tdCount);

        tbody.appendChild(row);
      });
    }
  </script>
</body>
</html>
```


The final result is not bad (with my weak level at front-end):

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-add-website-analytics-using-only-nginx/metric.png)

---

Thank you for reading this long blog post, am trying to post more often, i hope you liked it,
don't hesitate to add comments down below if you have any questions or remarks.

Thanks, see you next week.


### SOME REAL LIFE GOOD ANALYZER THAT EXIST

**NOTE:** None of them was writen in pure bash :)

- [Top 7 nginx analyzer](https://sematext.com/blog/nginx-log-analyzers/)
- [ngtop](https://github.com/facundoolano/ngtop)
- [nginx-log-analyzer](https://github.com/fantasticmao/nginx-log-analyzer)


-----------
