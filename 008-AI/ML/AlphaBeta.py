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

# Terminal utility values at the leaf nodes
leaf_scores = {
    'D': 3,
    'E': 5,
    'F': 6,
    'G': 9
}

def minimax_alpha_beta(node, is_maximizing_player, graph, scores, alpha=float('-inf'), beta=float('inf')):
    """
    Minimax implementation with Alpha-Beta pruning recursively evaluating a game tree.
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
            eval_value = minimax_alpha_beta(child, False, graph, scores, alpha, beta)
            best_value = max(best_value, eval_value)
            alpha = max(alpha, best_value)
            
            # Alpha-Beta Pruning Condition
            if beta <= alpha:
                print(f"✂ Pruning remaining branches at '{node}' (alpha={alpha}, beta={beta})")
                break
                
        print(f"✓ Maximizer at '{node}' chooses best value: {best_value}")
        return best_value

    else:
        best_value = float('inf')
        print(f"Minimizer at '{node}' -> evaluating children: {graph[node]}")
        
        for child in graph.get(node, []):
            eval_value = minimax_alpha_beta(child, True, graph, scores, alpha, beta)
            best_value = min(best_value, eval_value)
            beta = min(beta, best_value)
            
            # Alpha-Beta Pruning Condition
            if beta <= alpha:
                print(f"✂ Pruning remaining branches at '{node}' (alpha={alpha}, beta={beta})")
                break
                
        print(f"✓ Minimizer at '{node}' chooses best value: {best_value}")
        return best_value

if __name__ == "__main__":
    print("Minimax with Alpha-Beta Pruning Traversal Path:\n")
    
    # Start at root node 'A'
    optimal_value = minimax_alpha_beta('A', True, game_tree, leaf_scores)
    
    print(f"\nFinal Result: Optimal value for root node 'A' is {optimal_value}")