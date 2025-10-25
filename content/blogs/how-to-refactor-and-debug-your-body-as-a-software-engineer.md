![Header Image](https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-hack-and-debug-your-body-as-a-software-engineer/hack-body.jpg)

## HOW TO REFACTOR YOUR BODY AS A SOFTWARE ENGINEER

`2025-10-25 08:24 PM` • 20 min read • **#body** **#hack** **#refactor** **#debug**

---

### WHAT'S NEW?

- First of all, thank you for all the support and attention brought to the previous post I made regarding the [CSV parser](https://sanixdk.xyz/blogs/how-i-accidentally-created-the-fastest-csv-parser-ever-made). I'm still working on it, I wasn't expecting it to blow up that much, we're already near 100 stars on the GitHub repository, and I wanted to say thank you again.
- Now the website has an RSS feed, you can get the link here: [feed-link](https://sanixdk.xyz/feed.xml)

### DISCLAIMERS

- I am not a professional coach, I know nothing as a professional
- All elements listed here are from my own experiments, my own body, my mistakes, don't expect some advice to be always true
- I will illustrate some of my points with random code snippets, in order to make it more understandable for any software engineer reading this

---

### BRUTE FORCE TECHNIQUE

Well, where do I start? You see, working on your body is like working on a side project, that little folder in your computer you created to have fun at the beginning, and time to time, either you're still incrementing on it or you just skip paying attention to it...
I can easily assimilate the workout to be like learning Vim and trying to work with it, you will feel like shit in terms of productivity and everything, questioning yourself on that choice every single day, but at some point you will find out it was worth it. You need to see working from home as working out at home is like dev on local, and push deploy is the gym sale. There's something about the environment that changes everything, just like how your code behaves differently in production versus localhost.

As a noob knowing nothing, I was going to the gym every day when I started...
See it like a programming problem you have to solve, the first approach is to brute force (well, most of the time). Let's say we have a matrix, a 2-dimensional array and you're asked to search for a specific item. Usually, the easiest solution would have been to loop until you find the item:

```python
found = False
for row in range(1, 101):
    for col in range(1, 101):
        if item == matrix[row][col]:
            found = True
```

This can be ok on 100... but not okay on 1 Million items...

This is how I started my journey in the gym, trying everything at the same time, by training the full body every day...

You see, I am also a fan of anime and manga (if you recognize the person in the top image of this blog post... you know what I mean, shoutout to [GAROU](https://onepunchman.fandom.com/wiki/Garou) from One Punch Man, which is a character I really like because his body evolves every time he fights an opponent).

So this is how I was splitting my gym... just like Saitama said "100 push-ups, 100 sit-ups, 100 squats and 10km runs, do it every single day"...

Oh boy, I vaguely remember the first days I decided I will start that craziness. At that time I was unable to do 10 push-ups without gasping out... so I split the 100 into 10x10 push-ups with some minutes to rest, same for the sit-ups, for squats I was at 20x5, and the run... oh boy, I was not even able to do 3km without my body starting to ask for reanimation... nights were pure nightmares for my whole body, I was lucky enough to be able to work remotely, because some mornings, I was unable to move an inch.. and just laying there... with my laptop on my stomach... coding with the small energy I got left in my hands...

The problem with this approach is that it's inefficient, just like our brute force algorithm. You're exhausting all your resources (energy, recovery ability, motivation) without really understanding what works and what doesn't. It's like trying to fix a bug by randomly changing lines of code until something works, you might eventually succeed, but you won't understand why, and you'll probably create more problems along the way.

```javascript
// as an idiot, my initial workout "algorithm"
while (!muscular) {
    do_everything();
    hope_for_results();
    // No error handling, no optimization, just brute force the body...
}
```

---

### THE RIGHT OPTIMIZATION THROUGH EDUCATION

So just like a dev's technical problem, there is a better solution to search an item in a long matrix, by using techniques like Binary Search in order to split faster and find the item easily. Just to take the previous example, a better solution would be:

```python
# assuming i sorted it at the begining...
row_len = len(matrix)
col_len = len(matrix[0])

# Start at the top-right corner of the matrix
row, col = 0, col_len - 1

while row < row_len and col >= 0:
    if matrix[row][col] == item:
        found = True
        break
    elif matrix[row][col] > item:
        col -= 1  # to move to the left cell
    else:
        row += 1  # to move to the down cell
```
which IMHO... is much much much faster.

So there we go, I started watching some YouTube tutorials, like everything I know/do today, skateboard, drawing, coding, they all started with a YouTube search on "How to...", exactly how you're about to learn a new programming language or any new tech stack you want to master or get certified in.

I then found some popular gym influencers like Greg Doucette (except for all the drama, that guy has a lot of advice... believe that), Mike Israetel (much more aggressive but also pinpoint advice), Jeff Nippard. There are many more out there, some are going to tell you the truth, others lie just to sell their supplements. It's like Stack Overflow answers, you need to check the upvotes, read the comments, and test things yourself before blindly following advice.

My first weeks were painful as hell. Some days I couldn't lift my arms to wash my hair, other days walking down stairs was a special kind of torture. But here's the thing, this pain is like compiler errors. It's your body's way of telling you something happened, something changed. The difference between good pain (muscle soreness from growth) and bad pain (injury) is like the difference between a warning and a critical error. You need to learn to read these signals.

---

### DEBUGGING YOUR BODY: LEARNING TO READ THE ERROR MESSAGES

#### KNOW YOUR LIMITS

You're a human being...

not a super saiyan (i mean... for most of us), the most important thing to know is your body limits and when to say "no... I can't do that weight",
because, even if you have the strongest mind of the MCU, your muscles do have limits, and knowing those boundaries helps you not get injured.

#### DEBUG YOUR BODY

When you're coding and something goes wrong, what do you do? You either use a debugger, or you go the hard way with print statements everywhere to know what and how everything works. Your body works the same way.

Using a coach (video or a random YouTube gym "people") is like using a proper debugger, they can set breakpoints in your form, watch your variables (weight, reps, technique), and help you step through each movement to find where things go wrong. But coaches are expensive, just like enterprise debugging tools, in my case it was YouTube videos, me watching, testing, trying, observing how to do some exercises like different pushups, situps and to develop which muscle groups...

```python
def workout_with_coach(exercise, weight, reps):
    coach.observe(exercise)
    while reps > 0:
        form_check = coach.evaluate_form()
        if form_check.is_incorrect():
            coach.correct_form()
            continue
        execute_rep()
        reps -= 1
    return coach.feedback()
```

Going solo is like debugging with print/console.log statements. You need to be more creative and observant, not that it doesn't work... but you have to be much more verbose:

```python
def debug_workout_solo(exercise, weight, reps):
    print(f"Starting {exercise} with {weight}kg")
    record_video()

    for rep in range(reps):
        print(f"Rep {rep + 1}: check, how does this feel?")
        if pain_detected():
            print("WARNING: Sharp pain detected, stopping")
            break
        if form_breaking():
            print("Form degrading, consider lowering weight")

    review_video_later()
    compare_with_youtube_tutorials()
```

The mirror at your bathroom is your constant output...

Your muscle soreness is your log file...

That weird clicking in your shoulder is a warning that needs investigation...

You need to learn the difference between "undefined behavior" (bad form that might work today but will cause problems later) and "expected output" (proper muscle fatigue after a good workout)... even if that comes with a lot of practice over time.

---

### NUTRITION AND RECOVERY

When I started the journey I was skinny... i mean, like extremely skinny IMHO, around 78kg in total (I'm 107kg now). From my research, in order to gain muscle, I needed to understand that my body is just an I/O system. What you put in determines what you get out... not trash tho, but back in the day, I was doing everything to gain a lot of weight... eating 2 pizza XXL per day, burgers, etc... but only on that BULKING phase.

Think of nutrition like managing memory in your programs.

I tried protein shakes, it was working well but I was having a lot of issues on maintaining the intake in a long run, so I was on proteins only time to time, not every day.

But you need to see protein as your heap allocation for building new muscle structures. Carbohydrates are your quick-access cache for immediate energy. Fats are your long-term storage, necessary for hormone production and overall system stability. And just like memory leaks can crash your program, nutritional deficiencies will crash your gains.

```javascript
class BodySystem {
    constructor(weight, height, age) {
        this.weight = weight;
        this.metabolicRate = this.calculateBMR(weight, height, age);
        this.maintenanceCalories = this.metabolicRate * 1.5;
    }

    bulkMode() {
        // Allocate extra resources for growth
        this.targetCalories = this.maintenanceCalories + 500;
        this.proteinGrams = this.weight * 2.2;
        console.log("Bulk mode: Prepare for increased resource consumption");
    }

    cutMode() {
        // Garbage collection mode, remove excess
        this.targetCalories = this.maintenanceCalories - 500;
        this.proteinGrams = this.weight * 2.5; // Higher to preserve muscle
        console.log("Cut mode: Optimizing for efficiency");
    }
}
```

But here's what nobody tells you about gaining weight as a skinny person, it's like trying to scale a system that was built for different specifications. Your stomach is used to processing small amounts, your appetite regulation is tuned for maintenance, and suddenly you're trying to force 3000+ calories through a system designed for 2000. It takes time to adapt, just like gradually scaling up server capacity, some nights I was just vomiting everything I ate in the day (yes... that's how badly I wanted to change).

Recovery is your garbage collection and system maintenance. Sleep is when your body runs its cleanup processes, repairs damaged tissues, and consolidates the "motor patterns" you learned during training. Skipping sleep to train more is like skipping garbage collection to process more requests, eventually, your system will crash.

---

### YOUR BODY AS A LONG-TERM PROJECT

As I said at the beginning, you need to see your body like a side project, but one that you can't abandon when it gets boring. It requires constant commits, MR/PR, regular refactoring, and occasional major version updates. Some days you'll push breaking changes (injuries), other days you'll have beautiful clean merges (perfect workouts where everything clicks).

The thing about fitness that's exactly like programming is that consistency beats intensity. Writing code for 30 minutes every day will make you a better programmer than doing one 10-hour session per week. Same with the gym, three solid 45-minute workouts beat one 3-hour marathon session.

```bash
# Your fitness git log should look like this:
git log --oneline
xxxx Monday: Upper body, bench press PR 85kg (or more if you are a freak)
xxxx Wednesday: Legs, squats feeling strong
xxxx Friday: Full body, good form throughout

# and maybe not like this:
xxx Monday: 3 hour destroy everything session...
[followed by 2 weeks of nothing due to burnout/injury]
```

Just like you wouldn't deploy to production without testing, don't attempt max weights without proper warm-up. Just like you version control your code, track your workouts. Just like you read documentation before using a new library, learn proper form before adding weight.

---

### WHEN YOUR PROGRESS THROWS A NULL POINTER EXCEPTION

Eventually, you'll hit plateaus, periods where despite doing everything "right", you see no progress. This is your body's equivalent of a performance bottleneck. Maybe you need to refactor your approach (change your program), maybe you need to adjust your resources (eat more/less), or maybe you just need to clear cache and restart (take a deload week).

These plateaus taught me patience in a way that debugging taught me persistence. Sometimes the solution isn't to work harder but to work smarter. Sometimes you need to step back, review your logs (workout journal), analyze your metrics (weight, reps, measurements), and find the bottleneck in your system.

---

### MY RETROSPECTIVE

After 3 years of treating my body like a coding project, here are the design patterns that actually work for me:

- Don't try to implement everything at once. Start with basic compound movements, nail the form, then add complexity.

- Your body is always in production. There's no rollback for injuries. Test new exercises with light weight first.

- What gets measured gets improved. Track your lifts, your weight, your calories. Data doesn't lie, but feelings do.

- Whether it's anatomy, nutrition science, or exercise mechanics, understanding the "why" makes the "how" much more effective.

- When something hurts or isn't working, don't ignore it. Debug it. Is it form? Recovery? Nutrition? Treat problems systematically.

- Just like you can't run modern games on a 10-year-old laptop without upgrades, you can't lift heavy without building the foundation first. Respect your current specs while working to upgrade them.


IMHO, the beautiful thing about approaching fitness like a software engineer is that we're already wired for systematic problem-solving, incremental progress, and debugging when things go wrong. We understand that complex systems require patience, that optimization is an iterative process, and that sometimes the elegant solution is better than the brute force approach.

Your body is the most complex system you'll ever work on, and unlike your code, you can't just spin up a new instance if you break this one. So treat it with the same respect you'd give to a production database, regular backups (rest days), careful maintenance (mobility work), thoughtful updates (progressive overload), and always, always have a rollback plan (deload weeks).

Remember: every expert was once a disaster. Every big lift started with an empty barbell. Every transformation began with someone who couldn't do a single push-up properly. The only difference between those who succeed and those who don't isn't talent or genetics, it's the willingness to treat failure as a learning opportunity rather than a reason to quit.

Now if you'll excuse me, it's leg day, and these squats aren't going to debug themselves.

```python
if __name__ == "__main__":
    gym_session = WorkoutSession("legs")
    try:
        gym_session.execute()
    except ExhaustedError:
        print("YESSS !!!!")
    finally:
        protein_shake.consume()
        rest.sleep(8 * 3600)  # 8 hours
```

---

*What's your experience with fitness as a developer? Have you found any interesting parallels between coding and training? Drop a comment or reach out, I'd love to hear your debugging stories from the gym!*

---

