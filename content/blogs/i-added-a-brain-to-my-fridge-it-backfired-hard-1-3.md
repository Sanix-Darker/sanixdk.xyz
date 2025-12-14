<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/head.png" />

## I ADDED A BRAIN TO MY FRIDGE... it BACKFIRED HARD! (PART 1/3)
`2025-12-14 03:30PM` • 7 min read • **#ai** **#llm** **#mcp** **#yolo** **#raspberrypi**

---

<div id="toc-container"></div>

---

### DISCLAIMERS

- This first part is much more like an introduction to the project, because I'm still working on it. Real technical parts/details are coming soon (if my fridge doesn't order 500kg of cheese first).
- This is a recreational project i am building since some time now, between parts, i can change my initial plans MULTIPLE times.

### INTROOOO

Oh boy, where do I even **start** with this one??

You see, I'm a **curious dude** (like TOO MUCH to the extent that my friends have stopped asking "why" when I mention my weekend projects). I usually like to test out a bunch of *weird* stuff and **bleeding-edge tech**. Think of the time I tried to build a maze game using [**DC**](https://en.wikipedia.org/wiki/Dc_(computer_program)), that calculator programming language from the 70s shipped with all Unix kernels that no one notices... Or that time where I attempted to build a password generator using **Brainfuck** as a *dynamic salt*. (You can [**read here**](https://sanixdk.xyz/blogs/how-to-make-a-password-generator-using-brainfuck-part-1-3) about how I nearly broke my brain with that one.)

Some call it "recreational programming", I call it "having fun the weirdest way possible" (and occasionally questioning my sanity at 3 AM).

This time, as is tradition, it all started with my greatest feature so far... **Being LAZY** (like a Bear in Winter hibernation mode... but all year round).

Don't get me wrong, this isn't the common, garden-variety laziness you know or have heard of. It's the kind of laziness that has become a **meme** across the developer community: the phenomenon where there's something you could do manually in 10 minutes, but you're so lazy you decide to **spend 10 hours** automating it instead... because "it's fun"... right? RRRIIIGGGGHHHHTTT??????

Yes, *that* kind of lazy. The kind that makes you write 2,000 lines of code to avoid writing a 5-item shopping list.

---

I really **hate** that feeling of driving (on my skateboard) back from the **supermarket** only to find out I missed a crucial item. Or worse, standing in front of my fridge at 10 PM, ready to cook that amazing pasta carbonara (not because it's the only think i can cook... but because it's simple and i like that meal). I've been craving all day, only to realize I forgot the bacon. Again. For the third time this month because i was stuck on a weird bug since 2 sprints.

So, I had a **weird idea**. With all the hype around **AI**, **Agents**, and **Model Context Protocol (MCP)**, it felt like the perfect time to start working on a "useful" (read: *useful for my sheer laziness*) project and **kill two birds with one stone** (no birds were harmed in the making of this project, but my bank account might be).

Hear me out on this, I'm not going to start drawing some graphics like on [n8n](https://n8n.io) or any other platform that allows you to draw and connect items with a mouse. I want to build my stuff by coding each step myself. It has always been the easiest way for me to learn new things, and this was the perfect occasion to do so! Plus, clicking and dragging is for people who have their life together, and clearly, I'm building an AI fridge instead.

For my "fridge workflow" (my PM will laugh at these words for sure), I don't keep an inventory of items I need to buy all the time, but my fridge knows better than me. It knows my usual staples, what's low, and what I'll need for specific meals based on what's left. But there's a problem: my fridge does one thing, and it does it *very* well: **lowering the temperature of my groceries**. (What did you expect? Cooking for me too? It's a simple and basic model... and I really like tools that do one single thing well... all multi-purpose tools piss me off... because at some point there's always something lacking. Looking at you, printer-scanner-fax machines.)

Since years of weird old projects and technical deep dives, I've racked up some knowledge in **electronics**, **programming**, and **software/hardware** in general. The thought I had was simple: Why not make my fridge **"smart"?** And by smart, I mean taking it to the **EXTREME**.

I wanted it to be able to make **decisions by itself**, based on context, at **ANY time without my intervention/interaction**. Things like:
- Ordering items based on internal inventory and my habits
- Making online purchases based on meals I want to cook
- Giving it access to my bank account to estimate budget for the month over items and internal capacity for the groceries
- Basically, free will... What could possibly go wrong, **right**? RIIIIGGHHHTTT??? (Narrator: *Everything. Everything could go wrong.*)

## THE "AUTONOMOUS REFRIGERATOR AGENT", my MASTER PLAN

So, here's the plan. First, I'd create a **joint bank account** (on Revolut) for it, deposit a **low budget** (a controlled burn, if you will, think of it as paying tuition for the "University of Bad Ideas"), and give my fridge access. The fridge should then be able to make purchases for items categorized as **"required"**, like water and fruit juice. And maybe the occasional impulse buy of ice cream or anything with proteins at 2 AM, because even fridges need comfort food, right?

### EYES (The Vision System)

**First**, we'll add **"eyes"** (a vision system) to the fridge...

In order to "make decisions", it needs to see what's inside. Here's where things get technically interesting (and by interesting, I mean I spent 6 hours debugging WiFi connectivity issues).


![esp32-cam](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/esp.jpg)


I'm using an **ESP32-CAM module**, a beautiful piece of engineering that costs less than a fancy coffee but can stream video over WiFi. The ESP32-CAM sits inside the fridge, connected to a powerbank (because running cables through the door seal seemed... unwise). It's programmed to:

1. **Wake up when the door opens** (using a simple magnetic reed switch)
2. **Capture a burst of images** (because one blurry photo of my sad lettuce isn't enough)
3. **Stream them over WiFi** to my Raspberry Pi

The **Raspberry Pi 4** (sitting outside the fridge, because it prefers room temperature) runs the heavy lifting. It's running a [**YOLO (You Only Look Once) model**](https://yolov8.com/), and yes, the irony of "YOLO" for a fridge AI isn't lost on me. YOLO v8, to be specific, fine-tuned on a dataset I painstakingly created by photographing groceries like some sort of food paparazzi.

For an basic overview, you can see it like this :

```python
# The ESP32 streams frames via HTTP (over Wifi of my internet box on LAN)
# The Pi processes them in real-time
def process_fridge_frame(frame):
    # Run YOLO inference...
    results = yolo_model(frame)

    # Extract detected items (approximate quantities if there is the possibility to count items...)
    inventory = extract_inventory(results)

    # Update the MCP resources with new inventory
    await update_inventory_state(inventory)

    # Check if we're running low on essentials
    if check_critical_items(inventory):
        await trigger_reorder_workflow()
```

The YOLO model can identify:
- **What items are present** (milk, eggs, that suspicious leftover from last week)
- **Approximate quantities** (using bounding box sizes and some creative math)
- **Freshness indicators** (that banana is looking a bit too brown, buddy)

### BRAIN (The MCP Server & Decision Engine)

**Then**, we'll add a **"brain"** (the decision-making system)...

The brain runs on the same Raspberry Pi, implemented as a **FastAPI Python backend** using the **Model Context Protocol (MCP)**. MCP is relatively new (and by new, I mean bleeding-edge enough that the documentation sometimes feels like reading ancient scrolls), but it provides a standardized way for LLMs to interact with external systems.


![rpi](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/rpi.jpg)

My fridge MCP server will exposes:

**Tools** (things the AI can DO):
- `check_inventory`, Query what's currently in the fridge
- `create_shopping_list`, Generate a list based on missing items (or required for a meal ?)
- `place_order`, Actually buy stuff (oh boy...)
- `check_bank_balance`, Make sure we're not broke (spoiler: we might be)
- `suggest_recipes`, Based on available ingredients

**Resources** (things the AI can READ):
- `fridge://inventory/current`, Real-time inventory from YOLO
- `shopping://history`, Past purchases (to learn my habits)
- `recipes://database`, A collection of recipes it can suggest
- `bank://balance`, Current budget status

### HANDS (The Action Layer)

For the purchasing part, I've integrated with:
- **Local grocery store**, For placing orders.
- **Uber Eats API**, For when I'm feeling fancy (or desperate).

The beauty (or horror) is that once an order is placed, I just get a **notification** when items arrive at my doorstep. No interaction needed. The fridge handled everything. It's like having a very cold, rectangular personal assistant.

![fridge](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/i-added-a-brain-to-my-fridge-it-backfired-hard-1-3/fridge.jpg)

So, just to resume a nominal use-case :

1. Door opens -> ESP32-CAM wakes up,
2. Images stream to the running Raspberry Pi,
3. YOLO (model) identifies items and quantities from images,
4. MCP server updates inventory resources (or if you want, the SQLite local database in the raspberrypi),
5. If items are low -> AI evaluates need for action,
5. If items are almost at their EOL -> AI evaluates need for action,
6. If budget allows -> Place order automatically (lord have mercy...),
7. I get notification: "Your fridge ordered 2L of milk. It'll arrive at 3 PM."
8. I question my life choices but appreciate not having to shop myself when items are almost finished.

### THE CHAT INTERFACE

I needed a way to communicate with this monstrosity (I mean, magnificent creation). Being **extremely BAD at frontend**... and i am not even joking on this, my CSS skills peaked at `text-align: center`, and not wanting to "vibe code" something that plays with my "money"... well, because i like making stuff myself... and second... i am EXTREMELY BROKE... I decided to use **Telegram bot chat**.

The Telegram bot acts as the interface between my lazy self and the fridge's brain:

```python
@bot.command("whats_for_dinner")
async def dinner_suggestion(update, context):
    # Check current inventory via MCP
    inventory = await mcp_client.read_resource("fridge://inventory/current")

    suggestion = await llm.complete(
        f"Given these ingredients: {inventory}, "
        f"suggest a dinner recipe. Be creative but realistic."
    )

    # Check if we need anything
    missing = await check_missing_ingredients(suggestion)

    if missing:
        await update.reply(
            f"I suggest: {suggestion}\n"
            f"But we're missing: {missing}\n"
            f"Should I order them? (y/n)"
        )
```

### THE LLM

As suggested, I decided to use something small and quite fast that could start as quickly as possible. **Ollama** is a great tool for playing with models locally. I'm running **Mistral 7B** on the Pi (yes, it's sweating a bit, but it works... most of the time). For simple decisions, it's more than enough. For complex meal planning... well, sometimes it suggests "ice cream soup...", i am not a good cooker but i can tell, that meal title is suspicious... but we're working on it.

## THE ELECTRONICS

I have my own way of doing things... which I think sometimes is simpler and maybe better for my needs. For building and flashing my Arduino code, I'm not using the Arduino IDE but my **VIM editor** (because real programmers use VIM, or so I tell myself while crying over forgotten `:qall!` commands like, look, it was a joke, you can code with a rock, as soon as it works, you're doing good.), and command line tools to compile and upload the source code to the chip over USB.

soooo, let's jump into it, this is an expected flow regarding the ESP32-CAM :

```cpp
void setup() {
    // we initialize camera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    // ... other pins config ...

    // then we Connect to WiFi
    WiFi.begin("sanix-darker-modem", password);

    // detect/grap door sensor interrupt
    pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(DOOR_SENSOR_PIN, doorOpenISR, FALLING);
}

void doorOpenISR() {
    xTaskNotifyGive(cameraTaskHandle);
}
```

The beauty of this setup is that the ESP32 stays in deep sleep most of the time, only waking when needed. Battery life? Surprisingly good. My powerbank can lasts about a week like this i expect, which is longer than most of my food stays fresh anyway, and i really need to test this much longer, the temperature may affect my ESP32-CAM on long run...

---

Sooo, What do you think? Should I continue down this path of giving my appliances consciousness, or should I maybe consult a therapist first?

Too late, the idea is there, my beer is loaded, and I'm thrilled to continue building this beautiful disaster. Stay tuned for Part 2, where I'll dive deep into the technical implementation, share the complete code, and probably explain how my fridge achieved sentience and started ordering 5L of milk for itself from a YOLO error.

Remember: Sometimes the best projects aren't the most practical ones, they're the ones that make you question your sanity while teaching you something new. And if nothing else, I now have a really expensive way to never run out of milk.

---

