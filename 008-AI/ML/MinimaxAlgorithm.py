# Define the game tree
game_tree = {
    'A': ['B', 'C'],
    'B': ['D', 'E'],
    'C': ['F', 'G'],
    'D': [],
    'E': [],
    'F': [],
    'G': []
}

# Minimax requires terminal utility values at the leaf nodes to evaluate outcomes
leaf_scores = {
    'D': 3,
    'E': 5,
    'F': 6,
    'G': 9
}

def minimax(node, is_maximizing_player, graph, scores):
    """
    Minimax implementation recursively evaluating a game tree.
    Prints the evaluation path and node decisions.
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
            eval_value = minimax(child, False, graph, scores)
            best_value = max(best_value, eval_value)
            
        print(f"✓ Maximizer at '{node}' chooses best value: {best_value}")
        return best_value

    else:
        best_value = float('inf')
        print(f"Minimizer at '{node}' -> evaluating children: {graph[node]}")
        
        for child in graph.get(node, []):
            eval_value = minimax(child, True, graph, scores)
            best_value = min(best_value, eval_value)
            
        print(f"✓ Minimizer at '{node}' chooses best value: {best_value}")
        return best_value

if __name__ == "__main__":
    print("Minimax Traversal Path:\n")
    
    # Start at root node 'A', assuming the first mover is the maximizing player
    optimal_value = minimax('A', True, game_tree, leaf_scores)
    
    print(f"\nFinal Result: Optimal value for root node 'A' is {optimal_value}")