The core logic of a Bayesian Network is that a node's state is entirely dictated by its parents. To find the probability of any node, you must account for all possible states of its parents. 

Here is the step-by-step logic for building the formulas.

### 1. One Parent (Unknown State)
**Scenario:** You want $P(\text{Child})$. You know the child has one parent, but you do not know the parent's state.
**Intuition:** Because the child depends on the parent, you calculate the probability of the child for *every possible state* of the parent, weighted by how likely the parent is to be in that state. 
**Rule:** Marginalization (Summing out the parent).
**Formula Construction:**
1. Identify the parent: $P(\text{Parent})$.
2. Multiply child's probability given the parent by the parent's probability: $P(\text{Child}|\text{Parent})P(\text{Parent})$.
3. Add this up for all states of the parent (True and False).

$$P(\text{Child}) = P(\text{Child}|\text{Parent})P(\text{Parent}) + P(\text{Child}|\neg\text{Parent})P(\neg\text{Parent})$$

### 2. Two Parents (Unknown States)
**Scenario:** You want $P(\text{Child})$. The child has two parents.
**Intuition:** Same logic as above, but you must account for *every possible combination* of the two parents. Two boolean parents mean 4 combinations (TT, TF, FT, FF).
**Rule:** Marginalization over multiple variables.
**Formula Construction:**
1. Identify the combinations: $(P1, P2)$, $(P1, \neg P2)$, $(\neg P1, P2)$, $(\neg P1, \neg P2)$.
2. Multiply the child's probability given a combination by the probability of that combination occurring.
3. Sum all 4 terms.

$$P(\text{Child}) = P(\text{Child}|P1, P2)P(P1, P2) + \dots \text{ (for all 4 combinations)}$$

**Crucial Step:** To get the joint probability of the parents $P(P1, P2)$, look at their relationship in the graph:
* If $P1 \rightarrow P2$, then $P(P1, P2) = P(P2|P1)P(P1)$.
* If they share a parent $A$, calculate their probabilities given $A$.

### 3. Conditional Probability (Forward Direction)
**Scenario:** $P(\text{Grandchild} | \text{Grandparent})$. You know an ancestor and want to find a descendant.
**Intuition:** Trace the path. The grandparent influences the parent, which influences the grandchild. You must find the probability of the intermediate node (the parent) given the grandparent first.
**Rule:** Chain rule. Sum out the intermediate nodes given the evidence.
**Formula Construction:**
1. Identify the intermediate node connecting them.
2. Sum the grandchild's probability given each state of the intermediate node, weighted by the intermediate node's probability *given the grandparent*.

$$P(\text{Grandchild}|\text{Grandparent}) = P(\text{Grandchild}|\text{Parent})P(\text{Parent}|\text{Grandparent}) + P(\text{Grandchild}|\neg\text{Parent})P(\neg\text{Parent}|\text{Grandparent})$$

### 4. Conditional Probability (Backward Direction)
**Scenario:** $P(\text{Cause} | \text{Effect})$. You observe a descendant and want to know the probability of an ancestor. 
**Intuition:** You cannot read this directly from a CPT. The CPTs go Cause $\rightarrow$ Effect. To flip the direction, you must use Bayes' Theorem.
**Rule:** Bayes' Theorem. Probability equals the joint probability divided by the probability of the evidence.
**Formula Construction:**
1. Put the joint probability (both cause and effect happening) in the numerator.
2. Put the probability of the evidence (effect happening) in the denominator.

$$P(\text{Cause}|\text{Effect}) = \frac{P(\text{Cause, Effect})}{P(\text{Effect})}$$

To solve the numerator $P(\text{Cause, Effect})$, use the standard chain rule of the network: $P(\text{Cause})P(\text{Effect}|\text{Cause})$.

---

### Universal Checklist for Any Question

When facing a new question, answer these three prompts to determine the formula:

| Step | Action | Consequence |
| :--- | :--- | :--- |
| **1** | Identify the target variable. | This is what you are solving for. |
| **2** | Identify the evidence (what is given). | If there is evidence, it is a conditional probability problem. If the evidence is an *effect* and the target is a *cause*, immediately apply Bayes' Theorem (Scenario 4). |
| **3** | Identify the direct parents of the target. | If the parents are unknown, you MUST use marginalization (Scenario 1 or 2) to sum them out. |



### 1. Marginal Probability: Finding the Probability of a Single Node

To find the probability of a node when its parent states are unknown, you must sum over all possible states of its ancestors (Marginalization). Let $C$ be the target child node.

**A. Single Parent ($P$)**
Sum out the single parent.
$$P(C) = \sum_{p} P(C|p)P(p)$$
$$P(C) = P(C|P)P(P) + P(C|\neg P)P(\neg P)$$

**B. Two Independent Parents ($P_1, P_2$)**
Sum out all four combinations of the independent parents.
$$P(C) = \sum_{p_1, p_2} P(C|p_1, p_2)P(p_1)P(p_2)$$

**C. Two Dependent Parents ($P_1 \rightarrow P_2$)**
Sum out both parents, but account for $P_1$'s influence on $P_2$.
$$P(C) = \sum_{p_1, p_2} P(C|p_1, p_2)P(p_2|p_1)P(p_1)$$

**D. Two Parents Sharing a Common Grandparent ($G \rightarrow P_1, G \rightarrow P_2$)**
Sum out the parents and the grandparent to accurately capture the joint probability of $P_1$ and $P_2$.
$$P(C) = \sum_{p_1, p_2, g} P(C|p_1, p_2)P(p_1|g)P(p_2|g)P(g)$$

---

### 2. Forward Conditional Probability: Child Given Grandparent

When evaluating a descendant based on a known ancestor, use the chain rule to sum out the intermediate nodes connecting them.

**Structure:** $G \rightarrow P \rightarrow C$
**Given:** $G$ is known. Find $P(C|G)$.
**Formula:** $$P(C|G) = \sum_{p} P(C|p)P(p|G)$$
$$P(C|G) = P(C|P)P(P|G) + P(C|\neg P)P(\neg P|G)$$

**Example Application:** Network: Spring ($S$) $\rightarrow$ Rain ($R$) $\rightarrow$ Worms ($Wo$).
Find the probability of Worms given it is Spring: $P(Wo|S)$.
$$P(Wo|S) = P(Wo|R)P(R|S) + P(Wo|\neg R)P(\neg R|S)$$

---

### 3. Backward Conditional Probability (Bayes) & Joint Probability

When you observe an effect and want to find the probability of its cause, reverse the network direction using Bayes' Theorem.

**Bayes' Theorem Formula:**
$$P(\text{Cause}|\text{Effect}) = \frac{P(\text{Cause, Effect})}{P(\text{Effect})}$$

To calculate the numerator, you must find the **Joint Probability** (the probability of events occurring simultaneously) by moving forward down the network.

**A. Joint Probability of 2 Variables (Cause $\rightarrow$ Effect)**
Multiply the prior probability of the cause by the conditional probability of the effect.
$$P(C, E) = P(C)P(E|C)$$

**B. Joint Probability of 3 Variables**
The formula depends strictly on the network structure.

* **Structure 1: Chain ($A \rightarrow B \rightarrow C$)**
    $$P(A, B, C) = P(A)P(B|A)P(C|B)$$

* **Structure 2: Common Cause ($A \rightarrow B$ and $A \rightarrow C$)**
    $$P(A, B, C) = P(A)P(B|A)P(C|A)$$

* **Structure 3: Common Effect ($A \rightarrow C \leftarrow B$)**
    $$P(A, B, C) = P(A)P(B)P(C|A, B)$$