# **Game Specification: "King's Court Intrigue"**

### **1. Overview**

"**King's Court Intrigue**" is a strategic simulation game where the player
immerses themselves in the intricate world of a medieval royal court. Players
can assume the role of any character within the court, from a lowly servant to a
powerful noble, with the ultimate goal of amassing as much power and influence
as possible. The game emphasizes replayability through dynamic interactions
driven by a combination of an LLM backend and traditional AI, ensuring no two
playthroughs are the same.

### **2. Design Goals**

- **Emergent Storytelling**: Create a rich narrative experience driven by player
  choices and AI character interactions.
- **Replayability**: Each playthrough should offer unique scenarios and
  outcomes.
- **Scalability**: The architecture should handle a growing number of in-game
  characters and interactions smoothly.
- **Modularity**: Systems should be designed to allow easy addition of new
  features and content.
- **Performance**: Optimize for low overhead in critical game loops.

### **3. Architecture Overview**

The game's architecture will use a **hybrid approach**, leveraging an **ECS
pattern** for parts that benefit from its modularity and scalability and a
**traditional object-oriented approach** for components that require more
complex, hierarchical relationships or do not fit neatly into ECS paradigms.

#### **3.1 Use of ECS**

The ECS architecture will be employed for:

- **Character Simulation**: Managing character attributes, statuses, and
  interactions.
- **Game World Entities**: Representing in-game objects, environmental elements,
  and items.
- **Systems That Process Large Numbers of Entities**: For performance-critical
  updates (e.g., AI routines, status effect processing).

#### **3.2 Standard Approach Components**

Traditional object-oriented patterns will be used for:

- **Game State Management**: Handling transitions between different game states
  (e.g., main menu, gameplay, events).
- **UI and Rendering**: Managing the GUI, particularly when using ImGui or other
  immediate-mode GUI frameworks.
- **LLM Integration**: Handling communication with the backend LLM, conversation
  flows, and narrative generation.
- **Specific High-Level Game Logic**: Complex interactions that involve few
  entities but require intricate logic (e.g., court events, decision trees).

---

### **4. Detailed System Design**

#### **4.1 Entity-Component-System (ECS) Design**

##### **Entities**

- **Character Entities**: Represent all characters in the game, including the
  player and NPCs.
- **Item Entities**: Game items that can be interacted with or possessed by
  characters.
- **Environment Entities**: Objects within the game world (e.g., documents,
  artifacts).

##### **Components**

1. **IdentityComponent**

   - **Data**: Name, title, role, faction, relationships.
   - **Purpose**: Stores identification details and relationships for
     characters.

2. **AttributesComponent**

   - **Data**: Stats like intelligence, charisma, strength, influence, wealth.
   - **Purpose**: Defines character abilities and resources.

3. **StatusComponent**

   - **Data**: Current statuses (e.g., healthy, wounded, imprisoned).
   - **Purpose**: Tracks temporary or persistent effects on characters.

4. **AIComponent**

   - **Data**: Behavioral parameters, goals, personality traits.
   - **Purpose**: Holds data for traditional AI-driven behaviors.

5. **LLMComponent**

   - **Data**: Context for LLM interactions, conversation history.
   - **Purpose**: Manages data required for LLM-driven dialogue generation.

6. **InventoryComponent**

   - **Data**: List of item entities possessed.
   - **Purpose**: Manages a character's items.

7. **LocationComponent**

   - **Data**: Current location within the game world.
   - **Purpose**: Tracks where an entity is within the game environment.

8. **RenderableComponent**

   - **Data**: References to sprites, textures.
   - **Purpose**: Contains rendering data for entities (if applicable).

##### **Systems**

1. **AISystem**

   - **Function**: Processes AIComponents to perform AI routines for NPCs.
   - **Responsibilities**:
     - Execute daily tasks.
     - Update goals based on game state.

2. **LLMSystem**

   - **Function**: Interfaces with the LLM backend to generate dialogue and
     decisions.
   - **Responsibilities**:
     - Handle communications between game entities and the LLM.
     - Update conversation contexts.

3. **StatusSystem**

   - **Function**: Applies and updates status effects on entities.
   - **Responsibilities**:
     - Process status durations.
     - Apply effects (e.g., healing, worsening conditions).

4. **RelationshipSystem**

   - **Function**: Manages relationships and interactions between characters.
   - **Responsibilities**:
     - Update relationships based on interactions.
     - Determine alliances and rivalries.

5. **InventorySystem**

   - **Function**: Manages item transfers and usage.
   - **Responsibilities**:
     - Handle giving/taking items.
     - Process item effects.

6. **EventSystem**

   - **Function**: Handles global events that affect multiple entities.
   - **Responsibilities**:
     - Trigger events (e.g., feasts, councils).
     - Manage event outcomes.

7. **RenderingSystem**

   - **Function**: Renders entities with RenderableComponents.
   - **Note**: May be minimal or only for illustrative purposes if the game is
     text-based.

#### **4.2 Standard Approach Components**

##### **Game State Management**

- Use a traditional **State Machine** pattern to handle different game states.
- **States**:
  - **MainMenuState**
  - **GameplayState**
  - **EventState** (e.g., handling special court events)
  - **PauseState**
- Each state manages its own logic and transitions, independent of the ECS.

##### **UI and Rendering**

- Utilize **ImGui** (or another UI library) for GUI elements.
- The UI is managed separately from the ECS to allow immediate-mode rendering
  and simplify event handling.

##### **LLM Integration**

- **LLMController** Class:
  - Handles communication with the LLM backend.
  - Manages conversation flows, prompting, and response handling.
- The LLMController works alongside the ECS but is not part of it, due to its
  unique requirements.

##### **High-Level Game Logic**

- **EventManager**:
  - Manages scripted events and complex interactions.
  - Uses traditional OOP to handle intricate logic that doesn't fit the ECS
    model.
- **Decision Trees and Dialogues**:
  - Implemented using standard patterns or specialized libraries for handling
    complex branching logic.

---

### **5. Detailed Component and System Descriptions**

#### **5.1 Character Entities and Components**

##### **IdentityComponent**

- **Fields**:
  - `std::string name`
  - `std::string title`
  - `std::string role` (e.g., Spymaster, Noble)
  - `std::string faction`
  - `std::vector<Entity> relationships` (entities representing other characters)
- **Usage**:
  - Identifies characters and their relations to others.
  - Used by systems to determine interactions.

##### **AttributesComponent**

- **Fields**:
  - `int intelligence`
  - `int charisma`
  - `int strength`
  - `int influence`
  - `int wealth`
- **Usage**:
  - Defines a character's abilities.
  - Used in calculations for success/failure of actions.

##### **AIComponent**

- **Fields**:
  - `PersonalityTraits traits` (e.g., ambitious, loyal)
  - `std::vector<Goal> goals`
- **Usage**:
  - Directs the behavior of NPCs.
  - Determines AI decision-making processes.

##### **LLMComponent**

- **Fields**:
  - `ConversationContext context`
  - `std::vector<std::string> conversationHistory`
- **Usage**:
  - Stores data needed for LLM interactions.
  - Contains personalization details for dialogue generation.

#### **5.2 Systems**

##### **AISystem**

- **Process**:
  - Iterate over entities with AIComponent.
  - Update goals and make decisions based on current game state.
  - Interact with other entities via events or actions.

##### **LLMSystem**

- **Process**:
  - For entities with LLMComponent, send relevant data to the LLM backend.
  - Receive responses and update conversation contexts.
  - Coordinate with the **LLMController**.

#### **5.3 Integration Points Between ECS and Non-ECS Components**

- **LLMController and LLMSystem**:
  - The LLMController handles low-level communication and prompting.
  - The LLMSystem interacts with the LLMController to send and receive messages
    for entities.

- **Game States and Systems**:
  - Game states trigger systems based on the current state.
  - For example, in **GameplayState**, the AISystem and LLMSystem are active.
  - In **MainMenuState**, these systems may be inactive.

- **UI Interactions**:
  - User inputs are handled via standard event callbacks.
  - UI actions may result in changes to entities/components managed by the ECS.

---

### **6. Game Flow and Mechanics**

#### **6.1 Main Menu**

- Handled by **MainMenuState**.
- Provides options to start a new game, load a game, view settings, or exit.
- Uses standard UI components (e.g., buttons) via ImGui.

#### **6.2 Starting a New Game**

- Transition to **GameplayState**.
- Initialize the game world:
  - Create player entity with selected role.
  - Generate NPC entities and assign roles.
  - Initialize entities' components (attributes, relationships, etc.).

#### **6.3 Gameplay Loop**

- **Event Handling**:
  - Process player inputs.
  - Handle interactions via UI.
- **System Updates**:
  - **AISystem**: NPCs perform actions.
  - **LLMSystem**: Generate dialogues and interactions.
  - **StatusSystem**: Update statuses.
  - **RelationshipSystem**: Modify relationships.
- **Rendering and UI**:
  - Display the current state of the game.
  - Show dialogues, options, and feedback.

#### **6.4 Interactions and Dialogues**

- When the player interacts with an NPC:
  - **LLMSystem** gathers necessary context (from components).
  - Passes data to **LLMController**.
  - Receives LLM-generated response.
  - Updates conversation history.
  - Displays dialogue via UI.

#### **6.5 Game Events**

- **EventManager** triggers special events (e.g., court meetings, festivals).
- Events can modify the game state or alter entities.
- May involve complex logic unsuitable for ECS, handled via standard OOP.

#### **6.6 Saving and Loading**

- Game state serialization includes:
  - ECS registry data (entities and components).
  - Non-ECS data (e.g., game state variables, LLM conversation histories).
- Implemented via standard serialization techniques.

---

### **7. LLM Integration**

#### **7.1 LLMController**

- **Responsibilities**:
  - Manage connections to the LLM backend.
  - Handle prompt templates and ensure proper context is sent.
  - Parse responses and extract actionable data or dialogue.
- **Implementation Notes**:
  - Not part of ECS due to complexity and external dependencies.
  - Encapsulates LLM interaction logic.

#### **7.2 LLM Workflow**

1. **Data Gathering**:
   - Collect relevant data from entities (attributes, statuses).
2. **Prompt Generation**:
   - Create prompts using templates and collected data.
3. **LLM Communication**:
   - Send prompts to the LLM backend.
   - Receive responses asynchronously if needed.
4. **Response Handling**:
   - Parse the LLM's output.
   - Update game state accordingly (e.g., modify relationships, trigger events).
5. **Display**:
   - Present dialogue to the player via the UI.

---

### **8. Asset Management**

#### **8.1 Asset Loader**

- Use a robust asset manager with reference counting (`std::shared_ptr`).
- Assets are loaded on demand and automatically unloaded when no longer in use.
- Asset paths are managed via constants or enums to prevent typos.

#### **8.2 Font Management (ImGui)**

- ImGui fonts are managed separately.
- Fonts are loaded once and shared across the UI components.
- Cleared only when necessary (e.g., when changing themes or on shutdown).

---

### **9. Conclusion**

This specification outlines how to structure "King's Court Intrigue" using a
hybrid architecture that combines ECS and traditional approaches. The ECS
pattern is applied to areas where it offers significant benefits in scalability
and performance, such as managing numerous characters and their interactions.
For more complex or unique systems (e.g., LLM integration, UI handling),
traditional object-oriented programming is used to maintain simplicity and
flexibility.

This approach strikes a balance between the robustness of ECS for entity
management and the practicality of standard patterns for systems that don't
align well with ECS. It helps ensure that as the game grows, the architecture
remains maintainable and adaptable to new requirements.

---

### **Next Steps**

- **Prototype Core Systems**: Begin by implementing prototypes of the ECS with a
  few components and systems to validate the architecture.
- **Develop LLM Integration**: Build the LLMController and test communication
  with the backend.
- **Design UI/UX**: Create mockups of the UI using ImGui and integrate with game
  states.
- **Iterate and Refine**: Use feedback from prototyping to refine the
  architecture and address any issues.
