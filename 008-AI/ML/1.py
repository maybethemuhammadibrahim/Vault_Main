from ortools.sat.python import cp_model

# Custom class to print feasible solutions
class SolutionPrinter(cp_model.CpSolverSolutionCallback):

    def __init__(self, variables):
        super().__init__()
        self.variables = variables
        self.solution_count = 0

    def on_solution_callback(self):
        self.solution_count += 1
        print(f"Solution {self.solution_count}: ", end="")

        for var in self.variables:
            print(f"{var.Name()} = {self.Value(var)}", end="  ")

        print()

# Create the model
model = cp_model.CpModel()

# Variables
a = model.NewIntVar(1, 3, 'x')
b = model.NewIntVar(1, 3, 'y')
c = model.NewIntVar(1, 3, 'z')

# Constraints
model.Add(a != b)
model.Add(b != c)
model.Add(a != c)

# Create solver
solver = cp_model.CpSolver()

# Enable multiple solutions
solver.parameters.enumerate_all_solutions = True

solution_printer = SolutionPrinter([a, b, c])

# Solve
status = solver.Solve(model, solution_printer)

# Print solver status
print("\nSolver Status:", solver.StatusName(status))

# Handle different statuses
if status == cp_model.OPTIMAL:
    print("Optimal solution found.")

elif status == cp_model.FEASIBLE:
    print("Feasible solution found.")

elif status == cp_model.INFEASIBLE:
    print("No solution exists.")

elif status == cp_model.UNKNOWN:
    print("Solver could not determine a solution.")

# If no solutions printed
if solution_printer.solution_count == 0:
    print("No feasible solution found.")



----

from ortools.sat.python import cp_model

class VarArraySolutionPrinter(cp_model.CpSolverSolutionCallback):
    """Print intermediate solutions."""
    def __init__(self, variables: list[cp_model.IntVar]):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self.__variables = variables
        self.__solution_count = 0

    def on_solution_callback(self) -> None:
        self.__solution_count += 1
        for v in self.__variables:
            print(f"{v}={self.Value(v)}", end=" ")
        print()

    @property
    def solution_count(self) -> int:
        return self.__solution_count

def search_for_all_solutions_sample_sat():
    """Showcases calling the solver to search for all solutions."""
    # Creates the model.
    model = cp_model.CpModel()
    
    # Creates the variables.
    num_vals = 3
    x = model.NewIntVar(0, num_vals - 1, "x")
    y = model.NewIntVar(0, num_vals - 1, "y")
    z = model.NewIntVar(0, num_vals - 1, "z")
    
    # Create the constraints.
    model.Add(x != y)
    
    # Create a solver and solve.
    solver = cp_model.CpSolver()
    solution_printer = VarArraySolutionPrinter([x, y, z])
    
    # Enumerate all solutions.
    solver.parameters.enumerate_all_solutions = True
    
    # Solve.
    status = solver.Solve(model, solution_printer)
    
    print(f"Status = {solver.StatusName(status)}")
    print(f"Number of solutions found: {solution_printer.solution_count}")

search_for_all_solutions_sample_sat()