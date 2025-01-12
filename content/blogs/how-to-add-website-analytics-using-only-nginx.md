![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/aki.jpg)

## HOW TO ADD WEBSITE ANALYTICS USING ONLY NGINX AND BASH

`2025-01-26 10:04` • 19 min read • **#bash** **#nginx** **#terminal**


### DISCLAIMER AS INTRO


Hi there, what's up ?

Today, am going to share with you a small tip... using only `bash` and `nginx` to add views on a website,

You may be wondering... *"why even DOING THAT ?"*, you see, long time ago, i decided to make my personal website using only C, yup, the one you're looking right now, and i used a lot of existing "markdown -> html" C code pre-existing to add my own sauce.

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/webb.png)

[SEE THE REPO](https://github.com/Sanix-Darker/sanixdk.xyz)

This was a really really really BAD idea....
did i said "really" enough ?

But hey, making mistakes makes you grow (Spoiler alert am still DUMB af, at this day).

So, the problem with building something html/markdown only is that you don't have JS involves... at tbh, that was the first point (not because i hate modern JS :wink:);  Therefore, it was a 'NO google analytics' for me... (it's basically a win win... i hate Google Tracking every single aspect of my life).

Was it worth it, NO, will i do that again, HELL YEAH!

ANYWAY... let's jump into it, shall we ?

---

### THE SERIOUS INTRO

Since i was just returning basic html, no "serious JS" involves, there were still a service that was handling all requests comming to my VPS, "nginx", i then decided to use it for that tracking task.

Adding a "views/visited" system based on Nginx typically involves configuring Nginx to basically just log requests(with enough infos), process those logs to count views, and possibly visualize the data.

---

### HOW TO 'ACTCHUALLY' DO THAT

#### ENABLE LOGGING IN NGINX

Nginx logs requests by default, but you may need to customize the log format to track relevant details, such as URLs or IP addresses.

Edit your Nginx configuration file, typically located at `/etc/nginx/nginx.conf` or `/etc/nginx/sites-available/<your-site>`.

```bash
$ vim /etc/nginx/nginx.conf
# or 'nano' as the editor, if you're a noob like me.
```

then in the `http` section,

```nginx
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

And "VOILA", the most important part is done, you now have all requests logs going to the path specified in your config,
in our case, */var/log/nginx/access.log*, it will looks like :

```bash
$ cat /var/log/nginx/access.log

18.220.122.122 - - [12/Jan/2025:17:15:19 +0000] "GET / HTTP/1.1" 400 666 "-" "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) Chrome/126.0.0.0 Safari/537.36" "147.182.205.116" "/"$
54.36.148.9 - - [12/Jan/2025:17:16:15 +0000] "GET /robots.txt HTTP/1.1" 200 719 "-" "Mozilla/5.0 (compatible; AhrefsBot/7.0; +http://ahrefs.com/robot/)" "sanixdk.xyz" "/robots.txt"$
51.222.253.18 - - [12/Jan/2025:17:16:16 +0000] "GET /blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3 HTTP/1.1" 200 7569 "-" "Mozilla/5.0 (compatible; AhrefsBot/7.0; +http://ahrefs.com/robot/)" "sanixdk.xyz" "/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3"$
124.44.90.81 - - [12/Jan/2025:17:22:41 +0000] "GET / HTTP/1.0" 404 162 "-" "curl/7.88.1" "147.182.205.116" "/"$
95.214.55.226 - - [12/Jan/2025:17:26:19 +0000] "GET / HTTP/1.1" 404 162 "-" "-" "147.182.205.116" "/"$
```

Yes, i know... WHAT THE HELL is that and how is it going to help me to add analytics on my website ?

CHILL Sangoku, we're going to explain that in the next section

---

#### COLLECT MORE LOGS AND PROCESS THEM

The access logs regroups all requests comming through nginx to access a specific target, you can then have informations like the ip address, the url targeted, the device used, the timing between each requests...

Using tools like `awk`, `grep`, or custom scripts to parse the logs can help us extract valuable metrics.


##### 1. BASIC REQUEST AND VISITOR METRICS

1. **Visits by URL**
   - **What it is**: Count how many times a specific URL (e.g., `/about`, `/contact`) is requested.
   - **How**: Include `$request_uri` in your log format and aggregate by that field.

2. **Unique IP Addresses**
   - **What it is**: Approximate unique “visitors” by counting distinct IP addresses.
   - **How**: Include `$remote_addr`. (Note that IP-based uniqueness can be inaccurate due to NAT or shared IPs.)

3. **Request Methods**
   - **What it is**: How many `GET`, `POST`, `PUT`, or `DELETE` requests your site receives.
   - **How**: Include `$request_method`.

4. **HTTP Status Codes**
   - **What it is**: Distribution of `200`, `301`, `404`, `500`, etc. responses.
   - **Why**: Identify error trends or frequent 404s.
   - **How**: Log `$status`.

5. **Referring URLs**
   - **What it is**: Where traffic is coming from (Google, social media, other websites).
   - **How**: Include `$http_referer` and aggregate or filter by domain.

6. **User Agents (Browsers/Devices)**
   - **What it is**: Breakdown of which browser (Chrome, Firefox, Safari) and device (mobile, desktop, tablet) visitors are using.
   - **How**: Include `$http_user_agent` and parse it with a tool/library (e.g., [ua-parser](https://github.com/ua-parser) or log analysis software).

---

##### **2. PERFORMANCE AND TIMING METRICS**

1. **Request Processing Time**
   - **What it is**: Total time from when Nginx reads the first byte of the request to when it writes the last byte of the response.
   - **How**: Use `$request_time`.

2. **Upstream Response Time**
   - **What it is**: How long your upstream service (e.g., PHP-FPM, Node.js) took to generate a response.
   - **How**: Use `$upstream_response_time`. Great for identifying bottlenecks in backend services.

3. **Time to First Byte (TTFB)**
   - **What it is**: Measures how quickly Nginx can serve the first byte of data in the response.
   - **How**: Often approximated by `$request_time - $upstream_response_time` or by capturing `$upstream_header_time` if available (requires the right config).

4. **Bytes Sent / Bandwidth**
   - **What it is**: How many bytes or total bandwidth your site is sending out.
   - **How**: Use `$body_bytes_sent` to track the size of each response.

5. **Requests per Second (RPS)**
   - **What it is**: Volume of requests to your server in a given time window.
   - **How**: Aggregate the logs over a time period (e.g., 1 second or 1 minute) and count the lines or use `$request_time` averages.

---

##### **3. GEOGRAPHIC AND DEVICE INSIGHTS**

1. **GeoIP or Geolocation**
   - **What it is**: Identify your visitors’ country, city, or region.
   - **How**: Use Nginx’s GeoIP/GeoIP2 module or parse IPs in a separate step with a geolocation database (e.g., MaxMind’s GeoLite2).
   - **Note**: You’ll store `$remote_addr` in logs and then enrich that data with city/country info.

2. **Browser / OS Breakdown**
   - **What it is**: Identify device OS (Windows, macOS, Linux, Android, iOS) and browser versions.
   - **How**: Parse `$http_user_agent` using tools that can detect device type, OS, and browser version.

3. **Mobile vs. Desktop**
   - **What it is**: High-level view of which portion of traffic comes from mobile devices vs. desktop.
   - **How**: Again, parse `$http_user_agent`. Simple checks can detect `Mobile`, `Tablet`, or `Desktop`.

---

##### **4. SECURITY-RELATED METRICS**

1. **Top IP Addresses (Potential Attackers)**
   - **What it is**: Identify the IPs with unusually high request counts (possible brute force or DDoS).
   - **How**: Aggregate by `$remote_addr`. Look for spikes in request volume from a single IP.

2. **HTTP Methods and Status Anomalies**
   - **What it is**: Track unusual methods (e.g., lots of `POST`, `PUT`, `DELETE`) or high error rates (4xx, 5xx).
   - **How**: Use `$request_method` and `$status`.

3. **Blocked Requests**
   - **What it is**: If you have Nginx rules or WAF modules that block requests, you can log that.
   - **How**: Custom log when a rule is triggered; store a special message in your log format (or separate log).

---

##### **5. SESSION AND ENGAGEMENT METRICS (APPROXIMATE)**

1. **Session Counts**
   - **What it is**: Approximate how many “sessions” occur by grouping requests from the same IP + User-Agent within a certain timeframe.
   - **How**: This requires log analysis with a custom script or a tool that can correlate multiple requests from the same IP and user agent, and consider a session timeout (e.g., 30 minutes).

2. **Time on Page**
   - **What it is**: The difference between the first and the last request for a user on a page, though tricky to measure precisely with server logs alone.
   - **How**: Typically requires correlating consecutive hits from the same visitor. Not as accurate as client-side analytics (no explicit “page unload” event), but a rough approximation can be done.

3. **Bounce Rate (Approximation)**
   - **What it is**: Percentage of sessions with only one request.
   - **How**: Count how many unique sessions had just 1 log entry. Again, approximate due to lack of JavaScript “interaction” events.

---

##### **6. ERROR AND TROUBLESHOOTING METRICS**

1. **Most Common 4xx Pages**
   - **What it is**: Identify which pages cause the most client errors (404 Not Found, 403 Forbidden).
   - **How**: Filter `$status` for 4xx and group by `$request_uri`.

2. **Most Common 5xx Errors**
   - **What it is**: Pinpoint failing URLs or upstreams with server errors (500, 502, 503).
   - **How**: Filter `$status` for 5xx codes and group by `$request_uri` or `$upstream_status`.

3. **Upstream Connection Issues**
   - **What it is**: If you use proxy_pass to upstream services, track failed connections or high `$upstream_response_time`.
   - **How**: Include `$upstream_status` or check logs for upstream connection failures.

---

##### **7. SSL/TLS INSIGHTS (IF USING HTTPS)**

1. **SSL Protocol and Cipher**
   - **What it is**: Which TLS version (TLS 1.2, 1.3) or cipher suites are being used by clients.
   - **How**: Include `$ssl_protocol` and `$ssl_cipher` in your log format.
   - **Why**: Helpful for auditing security settings and ensuring deprecated protocols aren’t widely used.

2. **Handshake Time**
   - **What it is**: Time spent establishing SSL handshake. (Supported via `$ssl_handshake_time` in newer versions or patch modules.)
   - **Why**: Diagnosing slow TLS negotiations or large round-trip latencies.

---

### LET's FINALLY CODE

You know i like bash...
I mean, i LOVE bash,
WHY ?

Well, it's should be obvious by now, but, whatever linux distro you got into...

Or you just installed, it's already there, no need to install `gcc`, or `python` or anything... Because it's the shell btw, so we're going to use it.

Let's break down each steps to the service collecting all the metrics we need.

---

#### FIRST, THE SOURCE CODE:

Each instruction is documented as much as possible, fill free to ask questions in the comment section.

```bash
#!/usr/bin/env bash
#
# nginx-analytics.sh - Collect Nginx log metrics and output JSON analytics.
#
# by dk (https://github.com/sanix-darker)

LOG_FILE="/var/log/nginx/access.log"
OUTPUT_FILE="/var/www/html/nginx-analytics.json"

# Safety check: Ensure LOG_FILE exists
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
    # So split on space.
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

# 3) Average request time ($request_time), assume it's field #10 in your custom log format
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
# Convert the above variables to JSON.
# We'll do minimal parsing to produce valid JSON arrays/objects.
# ------------------------------------------------------------

# Helper function to convert "count value" lines into JSON array entries
# e.g., "123 /home" -> { "value": "/home", "count": 123 }
function lines_to_json_array() {
  local input="$1"
  local result="["
  local first=1
  while IFS= read -r line; do
    # split line into "count" and "value"
    count=$(echo "$line" | awk '{print $1}')
    value=$(echo "$line" | awk '{print $2}')
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

# visits_by_url -> JSON array
visits_by_url_json=$(lines_to_json_array "$visits_by_url")

# top_ips -> JSON array
top_ips_json=$(lines_to_json_array "$top_ips")

# top_status_codes -> JSON array
top_status_codes_json=$(lines_to_json_array "$top_status_codes")

# Build final JSON object
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

exit 0
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

For my use case i made a small html view to consume it that looks like this :

![image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/nginx-analyzer.jpg)

Thank you for reading this long blog post, am trying to post more often, i hope you liked it,
don't hesitate to add comments down below if you have any questions or remarks.

Thanks, see you next week.


### SOME REAL LIFE GOOD ANALYZER THAT EXIST

**NOTE:** None of them was writen in pure bash :)

- [Top 7 nginx analyzer](https://sematext.com/blog/nginx-log-analyzers/)
- [ngtop](https://github.com/facundoolano/ngtop)
- [nginx-log-analyzer](https://github.com/fantasticmao/nginx-log-analyzer)


-----------
