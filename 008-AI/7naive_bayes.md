Here are detailed, structured notes on the Naive Bayes classifier, designed to decode the specific mechanics of your provided exam question.

## 1. The Core Concept

Naive Bayes is a classification algorithm based on Bayes’ Theorem. It predicts the probability of a data point belonging to a specific class (like "Spam" or "Regular") based on the features it contains (like specific words).



### The Terminology (Plain English)
* **Prior Probability $P(\text{Class})$:** Your baseline belief before looking at the specific email. If 1 in 10 emails you receive is spam, your prior for spam is 0.1.
* **Likelihood $P(\text{Features} | \text{Class})$:** If you already know an email is spam, what is the probability it contains this specific combination of words?
* **Evidence $P(\text{Features})$:** The overall probability of seeing this specific combination of words across *all* emails. 
* **Posterior Probability $P(\text{Class} | \text{Features})$:** The final answer you want. *Given* these specific words, what is the probability the email is spam?

### The "Naive" Assumption
The algorithm is called "naive" because it assumes every feature is completely independent of every other feature. In reality, the word "free" and the word "money" often appear together. Naive Bayes ignores this correlation and treats their occurrences as isolated events, simplifying the math to straight multiplication.

---

## 2. Unpacking the Math Mechanics

To solve problems of this type, you must understand three specific mechanical rules.

### A. Binary Vector Representation (Bernoulli Event Model)
When an email is represented as a binary vector (e.g., `1 0 1`), the algorithm checks for the **presence AND absence** of words. 
* If a word is present, you use its probability: $P(w = 1 | \text{Class})$
* If a word is absent, you must use the probability of its absence: $1 - P(w = 1 | \text{Class})$
* *Crucial Note:* Words in a test sentence that are not in your trained vocabulary (like "psychology" or "for") are strictly ignored.

### B. Add-One (Laplace) Smoothing
If a word never appears in the spam training data, standard math would assign it a probability of 0. Because Naive Bayes relies on multiplication, a single 0 wipes out the entire equation. Smoothing prevents this by adding "fake" counts.

For a binary feature (present or absent), the formula is:
$$P(x_i = 1 | C) = \frac{\text{Count of } x_i \text{ in } C + 1}{\text{Total instances of } C + 2}$$

* **+1 in the numerator:** The "pseudo count" of one.
* **+2 in the denominator:** The number of possible states for the feature (it can either be 0 or 1, hence 2 states).

### C. The Full Prediction Formula
To find the probability that a sentence $s$ is Spam:
$$P(\text{Spam} | s) = \frac{P(\text{Spam}) \times P(s | \text{Spam})}{P(\text{Spam}) \times P(s | \text{Spam}) + P(\text{Regular}) \times P(s | \text{Regular})}$$

---

## 3. Step-by-Step Problem Breakdown

Here is how the concepts apply directly to the provided exam questions.

### Part A: Priors and Smoothing
**1. Why is a prior of $P(\text{Spam}) = 0.1$ sensible?**
In classification, you must weigh the cost of errors. A false positive (sending a vital regular email to the spam folder) is highly destructive. A false negative (seeing a spam email in your inbox) is a minor annoyance. Setting a very low prior for spam mathematically forces the algorithm to require overwhelming evidence (a very high likelihood) before it flags an email as spam.

**2. Calculating the Smoothed Parameters**
Let's look at the training data totals:
* Total Spam = 8
* Total Regular = 4

Count the occurrences of "study" in Spam: It appears 0 times. Apply the formula:
$$P(\text{study} | \text{Spam}) = \frac{0 + 1}{8 + 2} = \frac{1}{10}$$

Count the occurrences of "study" in Regular: It appears 3 times. Apply the formula:
$$P(\text{study} | \text{Regular}) = \frac{3 + 1}{4 + 2} = \frac{4}{6} = \frac{2}{3}$$

This logic repeats for every word, generating the exact list of probabilities provided in the prompt.

### Part B: Calculating the Posterior Probability
**Target Sentence:** "money for psychology study"
**Vector mapping:**
* study: 1
* free: 0 (absent)
* money: 1
* "for" / "psychology": Ignored (not in our 3-word vocabulary).

**Step 1: Calculate Likelihood for Spam**
$$P(s | \text{Spam}) = P(\text{study}=1|\text{Spam}) \times P(\text{free}=0|\text{Spam}) \times P(\text{money}=1|\text{Spam})$$
Because "free" is absent, we use $(1 - P(\text{free}=1|\text{Spam}))$.
$$P(s | \text{Spam}) = \left(\frac{1}{10}\right) \times \left(1 - \frac{9}{10}\right) \times \left(\frac{1}{2}\right)$$
$$P(s | \text{Spam}) = \frac{1}{10} \times \frac{1}{10} \times \frac{1}{2} = \frac{1}{200}$$

**Step 2: Calculate Likelihood for Regular**
$$P(s | \text{Regular}) = P(\text{study}=1|\text{Reg}) \times P(\text{free}=0|\text{Reg}) \times P(\text{money}=1|\text{Reg})$$
$$P(s | \text{Regular}) = \left(\frac{2}{3}\right) \times \left(1 - \frac{1}{3}\right) \times \left(\frac{1}{3}\right)$$
$$P(s | \text{Regular}) = \frac{2}{3} \times \frac{2}{3} \times \frac{1}{3} = \frac{4}{27}$$

**Step 3: Combine using Bayes' Theorem**
Using the given prior $P(\text{Spam}) = 0.1$, which means $P(\text{Regular}) = 0.9$. Let $p = P(\text{Spam})$.
$$P(\text{Spam} | s) = \frac{p \times \frac{1}{200}}{\left(p \times \frac{1}{200}\right) + \left((1 - p) \times \frac{4}{27}\right)}$$
Substitute $p = 0.1$:
$$P(\text{Spam} | s) = \frac{0.0005}{0.0005 + 0.1333} = 0.003736$$
There is roughly a 0.37% chance this specific email is spam under the current prior.

### Part C: Reverse Engineering the Prior
The question asks: What must $P(\text{Spam})$ be so that the final outcome $P(\text{Spam} | s) = 0.5$?

This is an algebra problem. We set the left side to 0.5 and leave $p$ as the variable to solve for.

$$0.5 = \frac{\frac{p}{200}}{\frac{p}{200} + \frac{4(1-p)}{27}}$$

Multiply both sides by the denominator:
$$0.5 \left( \frac{p}{200} + \frac{4}{27} - \frac{4p}{27} \right) = \frac{p}{200}$$

Divide both sides by 0.5 (which is the same as multiplying the right side by 2):
$$\frac{p}{200} + \frac{4}{27} - \frac{4p}{27} = \frac{2p}{200}$$

Subtract $\frac{p}{200}$ from both sides:
$$\frac{4}{27} - \frac{4p}{27} = \frac{p}{200}$$

Isolate the terms with $p$ on one side:
$$\frac{4}{27} = \frac{p}{200} + \frac{4p}{27}$$

Find a common denominator (5400) to combine the $p$ terms:
$$\frac{4}{27} = \frac{27p}{5400} + \frac{800p}{5400}$$
$$\frac{4}{27} = \frac{827p}{5400}$$

Solve for $p$:
$$p = \frac{4 \times 5400}{27 \times 827}$$
$$p = \frac{800}{827} \approx 0.96735$$

To make the algorithm completely unsure (50/50 split) about this specific sentence, you would need to hold a massive prior assumption that roughly 96.7% of all your incoming mail is spam.