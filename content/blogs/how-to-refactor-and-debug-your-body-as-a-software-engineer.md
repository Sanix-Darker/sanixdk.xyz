<img src="https://cdn.jsdelivr.net/gh/sanix-darker/sanixdk.xyz@master/content/assets/how-to-refactor-and-debug-your-body-as-a-software-engineer/hack-body.jpg" />

## HOW TO REFACTOR YOUR BODY AS A SOFTWARE ENGINEER

`2025-10-25 08:24 PM` • 12 min read • **#body** **#hack** **#refactor** **#debug**

---

<div id="toc-container"></div>

---

### WHAT'S NEW?

- Thanks for the support on my [CSV parser post](https://sanixdk.xyz/blogs/how-i-accidentally-created-the-fastest-csv-parser-ever-made). The project is still moving, and the repository is getting close to 100 stars.
- The site now has an [RSS feed](https://sanixdk.xyz/feed.xml).

### DISCLAIMERS

I am not a coach, doctor, or dietitian. This post describes my own experiments and mistakes. It is not a training or medical plan. Stop when you feel sharp pain, and ask a qualified professional about injuries, medical conditions, or major diet changes.

The code below is only a way to explain the idea. Your body is not a program, and an exercise is not a function with perfectly predictable output.

---

### MY FIRST APPROACH: DO EVERYTHING

When I started going to the gym, I used the same method I often used on programming problems: try the obvious solution first and worry about optimization later.

I trained my whole body every day. I wanted progress immediately, so I copied a routine inspired by Saitama from *One-Punch Man*: 100 push-ups, 100 sit-ups, 100 squats, and a 10-kilometre run every day. I also liked Garou's constant adaptation in the series, although my own adaptation was much less dramatic.

I could not do 100 push-ups in one set. I broke them into ten sets of ten. I split the sit-ups and squats in the same way. Running was worse. Three kilometres already felt like a serious test.

The first week left me sore enough to make normal tasks difficult. I was working from home, so I could sometimes code from bed when my legs did not want to cooperate. This was not a clever training method. It was too much work with no plan for recovery.

The routine looked like this:

```javascript
while (!muscular) {
    do_everything();
    hope_for_results();
}
```

The problem was not effort. I had no useful feedback. I changed too many variables at once, so I could not tell which exercise, amount of food, or amount of rest helped. If something hurt, I usually treated it as a challenge instead of a signal to stop and investigate.

---

### LEARN BEFORE YOU OPTIMIZE

I started learning instead of copying routines. I watched videos, read about basic exercise technique, and compared advice from different sources. Greg Doucette, Mike Israetel, and Jeff Nippard were among the people I watched. I did not accept every claim. Fitness advice has the same problem as programming advice: popularity is not proof.

I looked for explanations, checked whether several sources agreed, and tested one change at a time. I also learned that a program must fit the person using it. A routine that works for an experienced lifter may be a bad starting point for someone who has not trained before.

My training became less dramatic. I used fewer exercises, lighter weights, and more rest. I kept notes about the exercise, weight, sets, reps, and how the session felt. This gave me enough information to make a reasonable change the next time.

In programming, a search algorithm should match the data. Training works in a similar way: the plan should match your current ability, time, equipment, and recovery. There is no prize for choosing the most complicated routine.

---

### DEBUGGING YOUR BODY

A debugging session starts with an observation. The same is true in training.

If a movement feels wrong, I ask a few basic questions:

- Did my form change as I got tired?
- Did I add weight too quickly?
- Did I sleep enough?
- Did I warm up properly?
- Is this normal effort, muscle soreness, or pain that means I should stop?

A coach can help because another person can see problems that are hard to notice yourself. I have mostly learned from videos and recordings of my own workouts. Recording a set lets me review my technique later instead of trusting a tired memory.

```python
def review_set(exercise, weight, reps):
    record_video()

    for rep in range(reps):
        if sharp_pain_detected():
            stop()
            return "ask a professional"
        if form_breaking():
            stop_or_reduce_weight()
            return "adjust next set"

    review_video()
    write_down(exercise, weight, reps)
    return "continue carefully"
```

The important part is not the fake function. It is the pause before adding more weight. A mirror, a video, and a training log can show technique. They cannot diagnose an injury. A strange click, swelling, persistent pain, numbness, or weakness deserves attention from a qualified professional.

Do not use pain as a test of discipline. Effort is part of training. Sharp or worsening pain is a reason to stop.

---

### FOOD IS NOT A COMPILER FLAG

I was very thin when I began. I weighed about 78 kilograms. I now weigh about 107 kilograms, but the change took years and was not a straight line.

At first I tried to gain weight by eating whatever was easy. There were days with two extra-large pizzas and burgers. That gave me calories, but it did not give me a good routine. I also made the mistake of pushing food too hard. On some nights I felt sick or vomited. That was not progress. It was a sign that I needed to stop forcing the process and get better advice.

Food supports training, but it is not a simple input-output system. Appetite, activity, sleep, health, and food quality all affect the result. Protein, carbohydrates, and fats each have useful roles, but no single number works for everyone. A dietitian can help if you need a precise plan or have a medical condition.

Protein shakes helped me when eating enough was difficult. I did not use them consistently, and I did not need them for every meal. They are a convenient food, not a shortcut.

Sleep matters just as much. When I slept badly, my sessions suffered. Recovery is not lost time. It is part of the training plan. Adding more work while ignoring sleep is like adding requests to a service that is already failing health checks.

---

### KEEP THE PROJECT SMALL ENOUGH TO CONTINUE

My best results came from making the routine repeatable. Three short sessions that I can perform every week are more useful than one extreme session followed by two weeks of soreness.

I track the basics:

```bash
git log --oneline
xxxx Monday: upper body, steady bench press
xxxx Wednesday: legs, controlled squats
xxxx Friday: full body, good technique
```

The log does not need to be impressive. It needs to be accurate. I record what I did, then try to improve one small part when recovery allows it. That may mean one more repetition, a small weight increase, cleaner form, or the same work feeling easier.

I also warm up before heavy work. I learn an exercise with a light weight before increasing it. I do not treat a personal record as a requirement for every session. A good session can be one where I practiced the movement and finished without aggravating an old problem.

---

### WHEN PROGRESS STOPS

Plateaus happen. My weight can stay the same for weeks. A lift can stop improving. Motivation can disappear even when the routine is sound.

I start by checking the log rather than changing everything. Am I actually training as often as I think? Has my sleep changed? Am I eating enough for my goal? Did I increase the workload too quickly? Is the exercise still appropriate?

Sometimes the answer is rest. A lighter week can help when fatigue has accumulated. Sometimes the answer is a smaller calorie change or a different exercise. Sometimes the answer is that I need help from a coach or health professional.

A plateau is information. It is not proof that the whole project failed.

---

### WHAT I WOULD DO DIFFERENTLY

After three years, I would start with a much smaller plan:

- Learn a few basic movements and use a weight that allows controlled repetitions.
- Train on a schedule that leaves room for sleep and normal life.
- Track sessions without turning every number into a judgment.
- Change one variable at a time when progress slows.
- Stop for sharp pain and get help when a problem does not settle.
- Eat enough regular food to support the goal instead of trying to force calories in one burst.

The software analogy helped me make training less mysterious. Logs helped me notice patterns. Small changes kept the routine manageable. The analogy also has limits: a body has bad days, changing needs, and risks that a program does not.

I still enjoy the engineering part of it. I like testing a routine, finding what causes a problem, and making a careful adjustment. I just do not want to optimize so aggressively that I forget the system belongs to a person.

The goal is a routine you can continue when motivation is ordinary.

---

*What's your experience with fitness as a developer? Have you found useful parallels between coding and training? Share your experience or your gym debugging stories.*

---
