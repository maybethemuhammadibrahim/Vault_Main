## 2.1 Agents and Environments

**What is an Agent?**
An agent is anything that observes its surroundings and then takes action within them.

* **Sensors:** How the agent takes in information (e.g., a human's eyes, a robot's cameras, a software bot's data feed).
* **Actuators:** How the agent affects its environment (e.g., a human's hands, a robot's wheels, a software bot's screen output).

**How Agents Process Information**

* **Percept:** The specific input the agent's sensors are picking up *right now*.
* **Percept Sequence:** The complete memory or history of *everything* the agent has ever perceived.

**How Agents Make Decisions**
There is a distinct difference between the theory of an agent and its actual code:

* **Agent Function (The Concept):** An abstract, mathematical rule that dictates which action to take based on a given percept sequence.
* *Analogy:* The "Law Book" that defines the ideal, perfect ruling for every situation.


* **Agent Program (The Reality):** The actual code or physical mechanism running inside the agent that makes those decisions happen in real-time.
* *Analogy:* The "Judge" who interprets the law book and applies it in a real courtroom.
* *(Note: A "Table-driven" program is a basic agent that just uses a giant, memorized cheat sheet to match percepts to actions. This is easy to build but impossible to scale for complex real-world problems).*



**Classic Example: The Vacuum-Cleaner World**

* **Percepts:** Where it is and what the state is (e.g., `[Location A; Dirty]`).
* **Actions:** Move Left, Move Right, Suck dirt, NoOp (Do nothing).

---

## 2.2 Good Behavior: The Concept of Rationality

**What is a Rational Agent?**
A rational agent is simply one that does the "right thing."

**How do we define the "Right Thing"?**
We use a **Performance Measure**, which is a metric that evaluates how desirable the environment is after the agent acts. A rational agent will always choose the action that maximizes this expected score.

* *Example:* For a vacuum agent, the performance measure shouldn't be "how much it moves around," but rather "how much dirt it cleans up in an 8-hour shift." You get exactly what you measure.

**Rationality vs. Perfection (Crucial Distinction)**

* **Perfection (Omniscience):** Requires knowing the *exact actual outcome* of an action before doing it. This is impossible in the real world.
* **Rationality:** Maximizes the *expected* outcome based only on the information the agent currently has. Rationality is realistic; perfection is not.

**The Role of Learning and Autonomy**
A purely rational agent does more than just react; it learns.

* **Exploration:** The agent actively tries to gather new information about its environment.
* **Autonomy:** An agent lacks autonomy if it just blindly follows the prior knowledge programmed by its designer. A truly rational agent must be **autonomous**—it learns from its own percepts to adapt to things its creator couldn't predict.
* *Example:* A programmed vacuum just sweeps the floor blindly. An *autonomous* rational vacuum learns that dirt usually appears near the front door at 5 PM and proactively waits there.
Here are your study notes for the next section. They are structured to logically break down how we define an AI's workspace (the environment) and the "brains" we can put inside the AI (the agent structure).

---

## 2.3 The Nature of Environments

Before we can design a rational agent, we have to define the exact problem it is trying to solve. In AI, we call this the **Task Environment**.

### The PEAS Framework

To fully understand an agent's task environment, we define its **PEAS**:

* **P**erformance Measure: How do we measure success? (e.g., safety, speed, profit).
* **E**nvironment: What is the agent interacting with? (e.g., roads, a chessboard, the internet).
* **A**ctuators: How does the agent take action? (e.g., steering wheel, robotic arm, screen display).
* **S**ensors: How does the agent perceive the world? (e.g., cameras, microphones, keyboards).

### The 7 Dimensions of an Environment

The design of an AI agent is almost entirely dictated by the type of environment it lives in. We categorize environments using these contrasting pairs:

1. **Fully Observable vs. Partially Observable**
* *Fully Observable:* The agent's sensors can see the *complete* state of the environment at all times. (e.g., Chess—you can see all the pieces). The agent doesn't need to rely on memory.
* *Partially Observable:* Parts of the environment are hidden or sensors are noisy/inaccurate. (e.g., Poker—you can't see the opponent's cards; or a self-driving car in thick fog).


2. **Single-Agent vs. Multi-Agent**
* *Single-Agent:* The agent is acting alone (e.g., solving a crossword puzzle).
* *Multi-Agent:* The agent must interact with other agents.
* *Competitive:* Chess (I win, you lose).
* *Cooperative:* Taxi driving (we all want to avoid crashing into each other).




3. **Deterministic vs. Stochastic**
* *Deterministic:* The *next* state of the world is 100% predictable based on the *current* state and the agent's action. (e.g., If I move my chess piece, I know exactly where it will land).
* *Stochastic:* There is randomness involved; outcomes are not guaranteed. (e.g., Real-world driving—a pedestrian might suddenly step into the road).


4. **Episodic vs. Sequential**
* *Episodic:* The agent's life is divided into brief, independent "episodes." The action taken in one episode does *not* affect the next. (e.g., An AI sorting images on an assembly line—sorting one image doesn't change the next image).
* *Sequential:* Current actions strictly dictate future options. Short-term actions have long-term consequences. (e.g., Chess or driving).


5. **Static vs. Dynamic**
* *Static:* The world stops and waits while the agent is "thinking" about its next move. (e.g., A crossword puzzle).
* *Dynamic:* The environment keeps changing while the agent is deliberating. (e.g., Taxi driving—if you take too long to decide to brake, you will crash).


6. **Discrete vs. Continuous**
* *Discrete:* There are a limited, clearly defined number of states and actions. (e.g., Chess—you have a specific number of legal moves).
* *Continuous:* The environment has fluid, infinite possibilities. (e.g., Taxi driving—the steering wheel can be turned to an infinite number of microscopic angles; speed is a continuous flow).



> **The Ultimate Challenge:** The real world is the hardest possible combination. It is partially observable, stochastic, sequential, dynamic, continuous, and multi-agent.

---

## 2.4 The Structure of Agents

Agents range from very simple to highly complex. Here are the four basic agent types, listed in order of increasing intelligence, plus a fifth type that can be combined with any of them.

### 1. Simple Reflex Agents

These are the most basic agents. They operate strictly on the **current percept** (what they see right now) and completely ignore the past.

* **How it works:** Uses simple "Condition-Action" rules (If X, then Y).
* *Example:* A basic vacuum agent: "If the floor is dirty -> Suck. If the floor is clean -> Move."
* *Flaw:* It has no memory. If the world is partially observable, it will easily get stuck in infinite loops.

### 2. Model-Based Reflex Agents

These agents have an internal memory. They keep track of the world using an **internal model** of how things change over time.

* **How it works:** It combines what it sees right now with what it remembers from the past to understand the current, unseen state of the world. Then, it triggers a reflex action.
* *Example:* A vacuum cleaner that remembers which rooms it has already cleaned so it doesn't clean them twice.

### 3. Goal-Based Agents

These agents don't just react; they think about the future. They have a specific **goal** they are trying to reach.

* **How it works:** Before acting, the agent asks, "What will happen if I do this action? Will it get me closer to my goal?" It plans sequences of actions.
* *Example:* A GPS navigation system. It knows your current state (location) and your goal (destination), and calculates a path to get there.

### 4. Utility-Based Agents

While a goal-based agent just wants to reach the finish line, a utility-based agent wants to reach the finish line in the *best possible way*.

* **How it works:** It uses a **Utility Function** to measure how "happy" or "desirable" a specific outcome is. It weighs the probabilities of different outcomes and chooses the action that yields the highest *expected* utility.
* *Example:* A GPS system that doesn't just find *any* route (Goal-based), but calculates the route that is the fastest, uses the least gas, and avoids tolls (Utility-based).

### 5. Learning Agents

Any of the four agents above can be turned into a Learning Agent. A learning agent can start with zero knowledge and learn how to operate by interacting with the environment. It is made of four main parts:

1. **Performance Element:** The part of the agent that actually chooses the actions (this is essentially the reflex, goal, or utility agent from above).
2. **Critic:** Observes the results of the action and tells the agent how well it did compared to the performance standard.
3. **Learning Element:** Takes the feedback from the Critic and makes improvements to the Performance Element so it does better next time.
4. **Problem Generator:** Suggests new, exploratory actions. It intentionally tries new things to gather informative experiences, rather than just doing what is currently considered "best."

---

## Problem Representation in AI

Before an AI can solve a problem using the agent structures above, the problem must be translated into a language the machine understands.

**What is Problem Representation?**
It is the process of transforming a messy, real-world situation into a clean, structured, mathematical format that a computer can process.

**Why is it important?**
Machines cannot look at the world the way humans do. To allow an AI to reason, learn, and make decisions, we must encode the world into data structures. This involves defining:

* **Objects:** The entities involved (e.g., cities on a map).
* **Relationships:** How objects connect (e.g., roads connecting the cities).
* **Constraints:** The rules of the environment (e.g., speed limits, one-way streets).
* **Goals:** The desired end state (e.g., the destination city).

> *Intuitive Example:* To an AI, a game of Tic-Tac-Toe isn't X's and O's on paper. The problem is represented as a 3x3 matrix of numbers, where blank spaces are 0, X's are 1, and O's are -1. The AI calculates mathematical operations on this matrix to decide its next move.
