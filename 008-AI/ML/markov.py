import numpy as np

# Define the states and transition matrix
states = ["Red", "Blue"]
transition_matrix = np.array([[0.5, 0.5], # From Red -> Red or Blue
[0.5, 0.5]]) # From Blue -> Red or Blue

# Function to simulate the Markov process
def simulate_markov_process(initial_state, num_steps):
    current_state = initial_state
    state_sequence = [current_state]
    for _ in range(num_steps):
        if current_state == "Red":
            next_state = np.random.choice(states, p=transition_matrix[0])
        else:
            next_state = np.random.choice(states, p=transition_matrix[1])
        state_sequence.append(next_state)
        current_state = next_state
    return state_sequence

# Simulate the process starting from "Red" and for 10 steps
initial_state = "Red"
num_steps = 10
state_sequence = simulate_markov_process(initial_state, num_steps)
