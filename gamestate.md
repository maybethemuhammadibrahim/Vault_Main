Here is the detailed, line-by-line explanation for the `GameButton` struct and the `GameScreen` class.

### Part 1: The Helper Struct (`GameButton`)

This is a lightweight container. Think of it as a custom data type that bundles a visual shape, some text, and an ID tag together.

```cpp
struct GameButton {
    // VISUALS
    // sf::RectangleShape is a built-in SFML class for drawing rectangles.
    // This represents the colored background of the button.
    sf::RectangleShape shape;

    // std::optional is a C++ feature. It means "This might have text, or it might be empty."
    // sf::Text is the SFML class for rendering strings using a loaded font.
    optional<sf::Text> text;

    // DATA
    // A simple string ID (e.g., "undo", "menu") so the code knows which button is which.
    string id;

    // A simple switch. If false, the button works visually but clicking does nothing.
    bool enabled;

    // CONSTRUCTOR
    // Initializes the button with defaults: no text (nullopt), empty ID, and enabled=true.
    GameButton() : shape(), text(nullopt), id(""), enabled(true) {}

    // COLLISION DETECTION FUNCTION
    // Input: A point (sf::Vector2f) representing where the mouse cursor is (x, y).
    bool contains(sf::Vector2f point) const {
        // 1. Check if the button is enabled. If not, ignore clicks.
        // 2. shape.getGlobalBounds() creates a virtual box (Rect) around the button.
        // 3. .contains(point) is an SFML math function: "Is this point inside that box?"
        
        return enabled && shape.getGlobalBounds().contains(point);
    }
};
```

-----

### Part 2: The GameScreen Class

This class controls the actual gameplay view. It translates the abstract grid of numbers (the board) into circles, lines, and squares on your screen.

#### 1\. Private Members (Settings & Tools)

```cpp
class GameScreen {
private:
    // REFERENCES (&)
    // We use '&' because we don't want to create a NEW window or font.
    // We want to use the specific Window and Font created in the 'Application' class.
    sf::RenderWindow& window;
    GameBoard& board;     // Reference to the logic grid (data).
    sf::Font& font;       // Reference to the loaded font file.

    // CONFIGURATION CONSTANTS (Styling)
    // "const" means these numbers cannot change while the game runs.
    const float TILE_SIZE = 100.0f;       // Each grid cell is 100x100 pixels.
    const float PIPE_THICKNESS = 8.0f;    // How thick the connection lines are.
    const float UI_HEIGHT = 100.0f;       // Space at the bottom for buttons.
    const float BUTTON_HEIGHT = 40.0f;    // Height of clickable buttons.
    const float BUTTON_SPACING = 10.0f;   // Gap between buttons.

    // COLORS
    // sf::Color takes (Red, Green, Blue) values from 0-255.
    const sf::Color BACKGROUND_COLOR = sf::Color(40, 40, 45);    // Dark Grey
    const sf::Color TILE_BG_COLOR = sf::Color(60, 60, 70);       // Slightly lighter grey
    const sf::Color CONNECTED_COLOR = sf::Color(50, 200, 50);    // Bright Green
    const sf::Color DISCONNECTED_COLOR = sf::Color(150, 150, 150); // Dull Grey
    const sf::Color HOVER_COLOR = sf::Color(80, 80, 100);        // Highlight color
    const sf::Color SERVER_COLOR = sf::Color(200, 50, 50);       // Red
    const sf::Color COMPUTER_COLOR = sf::Color(50, 150, 255);    // Blue
    const sf::Color BUTTON_COLOR = sf::Color(70, 130, 180);      // Steel Blue
    const sf::Color BUTTON_HOVER_COLOR = sf::Color(100, 160, 210); // Lighter Blue

    // STATE VARIABLES
    // List of all buttons on this screen.
    vector<GameButton> buttons;
    
    // Tracks which tile the mouse is currently over (Row, Col). {-1, -1} means "none".
    pair<int, int> hoveredTile = { -1, -1 };
    
    // A flag to tell the Application class: "The user wants to go back to the menu".
    bool returnToMenu = false;
```

#### 2\. The Constructor

Sets up the screen when it is first created.

```cpp
public:
    // Constructor receives the Window, Board, and Font from the main Application.
    GameScreen(sf::RenderWindow& win, GameBoard& gameBoard, sf::Font& gameFont)
        : window(win), board(gameBoard), font(gameFont) {
        // Immediately calculate where the buttons should go.
        setupButtons();
    }
```

#### 3\. Handling Inputs (`handleEvents`)

This translates raw inputs (mouse/keyboard) into game actions.

```cpp
    void handleEvents() {
        // Standard SFML Event Loop. "pollEvent" checks the queue for clicks/keypresses.
        while (const std::optional event = window.pollEvent()) {
            
            // If the user clicked the OS "X" button...
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // If the mouse was clicked...
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                // Check if it was the Left Mouse Button.
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    // Call our helper function with the (X, Y) pixel coordinates.
                    handleMouseClick(sf::Vector2f(
                        static_cast<float>(mousePressed->position.x),
                        static_cast<float>(mousePressed->position.y)));
                }
            }
            // If the mouse moved (used for hover effects)...
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                handleMouseMove(sf::Vector2f(
                    static_cast<float>(mouseMoved->position.x),
                    static_cast<float>(mouseMoved->position.y)));
            }
            // If a keyboard key was pressed...
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                // If the key was ESCAPE...
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    // Set the flag so the main app knows to switch screens.
                    returnToMenu = true;
                }
            }
        }
    }
```

#### 4\. Logic Updates (`update`)

Runs every frame to update button states based on game rules.

```cpp
    void update() {
        // Iterate through all buttons.
        for (auto& btn : buttons) {
            // If it's the "Undo" button...
            if (btn.id == "undo") {
                // Ask the logic board: "Do we have moves to undo?"
                btn.enabled = board.canUndo();
                // If enabled, make it Blue. If disabled, make it Dark Grey.
                btn.shape.setFillColor(btn.enabled ? BUTTON_COLOR : sf::Color(50, 50, 50));
            }
            // Same logic for the "Redo" button.
            else if (btn.id == "redo") {
                btn.enabled = board.canRedo();
                btn.shape.setFillColor(btn.enabled ? BUTTON_COLOR : sf::Color(50, 50, 50));
            }
        }
    }
```

#### 5\. Drawing to Screen (`render`)

This is the "Painter" function.

```cpp
    void render() {
        // 1. Wipe the screen clean with the background color.
        window.clear(BACKGROUND_COLOR);

        // 2. Loop through every row and column of the grid logic.
        for (int row = 0; row < board.getRows(); row++) {
            for (int col = 0; col < board.getCols(); col++) {
                // Draw the specific graphic for this tile (Pipe, Server, or Computer).
                drawTile(row, col);
            }
        }

        // 3. Draw the text at the bottom (Score, Win status).
        drawUI();

        // 4. Draw the buttons on top of everything else.
        for (auto& btn : buttons) {
            window.draw(btn.shape); // Draw the box
            if (btn.text) {
                window.draw(*btn.text); // Draw the label
            }
        }

        // 5. Flip the buffer. This takes everything we just drew in memory 
        // and pushes it to the actual monitor.
        window.display();
    }
```

#### 6\. Helper: Mouse Logic (`handleMouseClick`)

Determines *what* the user clicked.

[Image of coordinate system mapping pixels to grid]

```cpp
    void handleMouseClick(sf::Vector2f mousePos) {
        // 1. CHECK BUTTONS
        for (auto& btn : buttons) {
            // If the mouse is inside a button...
            if (btn.contains(mousePos)) {
                // Perform the action (New Game, Reset, etc.)
                handleButtonClick(btn.id);
                return; // Stop checking, we found the click.
            }
        }

        // 2. CHECK GRID (If we didn't click a button)
        // Check if the click is within the grid area (Y-axis check).
        if (mousePos.y < board.getRows() * TILE_SIZE + 20) {
            // MATHEMATICAL MAPPING:
            // Convert Pixels (e.g., 250px) to Grid Index (e.g., Column 2).
            // Logic: (250 - 20 padding) / 100 size = 2.3 -> cast to int -> 2.
            int col = static_cast<int>((mousePos.x - 20) / TILE_SIZE);
            int row = static_cast<int>((mousePos.y - 20) / TILE_SIZE);

            // Safety check: Ensure indices are actually inside the grid.
            if (row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols()) {
                // Tell the logic board to rotate this piece.
                board.rotateTile(row, col);
            }
        }
    }
```

#### 7\. Helper: Drawing a Tile (`drawTile`)

This function looks at the logic data and figures out which shapes to draw.

```cpp
    void drawTile(int row, int col) {
        // Get the data for this specific spot (e.g., "It's a corner pipe, rotated 90 deg").
        const Tile& tile = board.getTile(row, col);
        
        // Calculate the top-left pixel position for this tile.
        float x = 20 + col * TILE_SIZE;
        float y = 20 + row * TILE_SIZE;

        // Draw the dark background square for the tile.
        sf::RectangleShape bg(sf::Vector2f(TILE_SIZE - 4, TILE_SIZE - 4));
        bg.setPosition({ x + 2, y + 2 });

        // If mouse is hovering over this specific tile, light it up.
        if (hoveredTile.first == row && hoveredTile.second == col) {
            bg.setFillColor(HOVER_COLOR);
        } else {
            bg.setFillColor(TILE_BG_COLOR);
        }
        window.draw(bg);

        // If it's an empty tile, stop here.
        if (tile.type == ' ') return;

        // Decide color: Green if connected to server, Gray if not.
        sf::Color pipeColor = tile.isConnected ? CONNECTED_COLOR : DISCONNECTED_COLOR;
        float center = TILE_SIZE / 2.0f;

        // DRAWING LOGIC FOR DIFFERENT TYPES
        if (tile.type == 'N') {
            // 'N' = Network Server. Draw a Red Square in the middle.
            sf::RectangleShape server(sf::Vector2f(40, 40));
            server.setPosition({ x + center - 20, y + center - 20 });
            server.setFillColor(SERVER_COLOR);
            window.draw(server);
            // Draw "S" text on top...
        }
        else if (tile.type == 'C') {
            // 'C' = Computer. Draw a Blue Circle.
            sf::CircleShape computer(20); // Radius 20
            computer.setPosition({ x + center - 20, y + center - 20 });
            computer.setFillColor(COMPUTER_COLOR);
            window.draw(computer);
            
            // Draw lines (pipes) coming out of the computer based on logic (up, down, left, right).
            if (tile.up) drawPipeSegment(x, y, center, 0, -center, pipeColor);
            // ... (repeated for other directions)
        }
        else {
            // Regular Pipe. Draw lines based on connections.
            if (tile.up) drawPipeSegment(x, y, center, 0, -center, pipeColor);
            // ... (repeated for other directions)

            // Draw a small circle in the center to make the joint look smooth.
            sf::CircleShape connector(PIPE_THICKNESS);
            connector.setPosition({ x + center - PIPE_THICKNESS, y + center - PIPE_THICKNESS });
            connector.setFillColor(pipeColor);
            window.draw(connector);
        }
    }
```
