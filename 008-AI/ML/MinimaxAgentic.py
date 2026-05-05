# 1. Environment State (Game Tree and Leaf Scores)
game_tree = {
    'A': ['B', 'C'],
    'B': ['D', 'E'],
    'C': ['F', 'G'],
    'D': [],
    'E': [],
    'F': [],
    'G': []
}

leaf_scores = {
    'D': 3,
    'E': 5,
    'F': 6,
    'G': 9
}

# 2. Agent Definition
class MinimaxAgent:
    def formulate_goal(self, root_node, optimal_value):
        return f"Goal reached. Optimal value for root node '{root_node}' is {optimal_value}"

    def act(self, node, environment):
        """The agent's action is to evaluate the tree and return the result."""
        print("Agent initiating Minimax Search...\n")
        
        # The agent uses the environment's graph and scores to make its computation
        optimal_value = self.minimax(node, True, environment.graph, environment.scores)
        
        return self.formulate_goal(node, optimal_value)

    def minimax(self, node, is_maximizing_player, graph, scores):
        """
        The exact intuitive algorithm provided previously.
        """
        # Base Case: Terminal/Leaf node reached
        if not graph.get(node):
            score = scores.get(node, 0)
            print(f"Leaf '{node}' evaluated to: {score}")
            return score

        if is_maximizing_player:
            best_value = float('-inf')
            print(f"Maximizer at '{node}' -> evaluating children: {graph[node]}")
            
            for child in graph.get(node, []):
                eval_value = self.minimax(child, False, graph, scores)
                best_value = max(best_value, eval_value)
                
            print(f"✓ Maximizer at '{node}' chooses best value: {best_value}")
            return best_value

        else:
            best_value = float('inf')
            print(f"Minimizer at '{node}' -> evaluating children: {graph[node]}")
            
            for child in graph.get(node, []):
                eval_value = self.minimax(child, True, graph, scores)
                best_value = min(best_value, eval_value)
                
            print(f"✓ Minimizer at '{node}' chooses best value: {best_value}")
            return best_value

# 3. Environment Definition
class Environment:
    def __init__(self, graph, scores):
        self.graph = graph
        self.scores = scores

    def get_percept(self, start_node):
        """Returns the current state/node to the agent."""
        return start_node

# 4. Execution Loop
def run_agent(agent, environment, start_node):
    # Agent perceives the starting state from the environment
    percept = environment.get_percept(start_node)
    
    # Agent acts upon the percept
    result = agent.act(percept, environment)
    
    print(f"\nFinal Result: {result}")

if __name__ == "__main__":
    # Initialize components
    env = Environment(game_tree, leaf_scores)
    agent = MinimaxAgent()
    
    # Run the simulation starting at root node 'A'
    run_agent(agent, env, 'A')