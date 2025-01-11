# 1. **Overview**

**Game Title**: *Courtly Intrigues* (Working Title)

**Genre**: Strategic Medieval Simulation

**Primary Goal**: Allow players to take on various roles in a medieval royal court—from servant to noble—to gain and wield power and influence. The game world is driven by both an **LLM-driven AI** and **traditional game AI**, ensuring emergent, replayable storylines that respond dynamically to player choices.

---

# 2. **Design Goals**

1. **Emergent Storytelling**  
   - Use AI-driven character interactions to create branching narratives.  
   - Ensure that the game naturally spawns new plots, alliances, and intrigues as players take different actions.

2. **Replayability**  
   - Guarantee that each playthrough feels unique, supported by dynamic AI behaviors and narrative events generated through an LLM backend.  
   - Provide multiple roles (servant, courtier, knight, noble, royal, etc.) that fundamentally alter gameplay experiences.

3. **Scalability**  
   - Efficiently handle large numbers of in-game characters with minimal performance degradation.  
   - Allow the game world to expand with new realms, new cast of characters, and new intrigues without significant rework.

4. **Modularity**  
   - Implement features so they can be replaced or upgraded without impacting the rest of the system.  
   - Facilitate addition of new story arcs, roles, and mechanics without breaking existing systems.

---

# 3. **Core Game Loop**

1. **Character Creation & Role Selection**  
   - The player chooses a role: from a lowly servant to a powerful noble.  
   - Different roles have unique starting attributes, resources, social networks, and objectives.

2. **Daily Actions & Court Interaction**  
   - Players perform day-to-day actions (e.g., petition the King, deliver messages, attend feasts, sabotage rivals).  
   - AI-driven court members respond dynamically to player actions.  
   - Emergent events can disrupt the court (e.g., new alliances, royal decrees, foreign threats).

3. **Plotting & Alliance Building**  
   - Form alliances or rivalries with other characters.  
   - Influence the court's social and political structure.  
   - The LLM backend generates and resolves complex political narratives based on in-game events.

4. **Progression & Consequences**  
   - Player choices lead to immediate and long-term consequences.  
   - Gaining more power can unlock new roles or storylines.  
   - The game can end in various ways—overthrowing the current ruler, being exiled, achieving the highest rank, etc.

5. **Emergent Conclusions & Replay**  
   - Multiple endings shaped by the player’s cumulative actions.  
   - Encourage replay to explore different roles, alliances, or storylines.

---

# 4. **Game Architecture**

## 4.1 Hybrid ECS/OOP Model

- **ECS (Entity Component System)** is employed where high-volume, reusable data transformations are needed (e.g., for large numbers of NPCs, each requiring frequent updates).
  - **Entities**: Represent each character or object in the game (e.g., NPC, building, item).  
  - **Components**: Data containers (e.g., StatsComponent, RelationComponent, LocationComponent).  
  - **Systems**: Logic that processes entities with specific components (e.g., AIBehaviorSystem, NeedsSystem).

- **OOP** is used for high-level game flow and specialized logic (e.g., storyline managers, quest/event scripting, UI flows).
  - **Classes**:  
    - `GameManager` (handles overarching game state and scenes).  
    - `StoryManager` (controls narrative generation & integration with the LLM).  
    - `Role` (abstract class for specialized roles like `NobleRole`, `ServantRole`, etc.).  

This hybrid architecture helps **performance** (with ECS for mass updates) while retaining **readability** and **maintainability** (with OOP for complex logic).

## 4.2 Data Flow Overview

```
         +-----------------+
         |     Player      |
         +--------+--------+
                  |
                  v
        +---------+----------+
        |  GameManager (OOP) |
        +---------+----------+
                  |
       ECS Systems + <----------> ECS Components
                  |
                  v
   +-----------------------------+
   |     LLM / AI Interaction   |
   |  (StoryManager, Dialogue)  |
   +-----------------------------+
```

1. **Player actions** trigger **GameManager** methods (OOP) and update ECS Components.  
2. **ECS Systems** (e.g., `RelationSystem`, `NeedsSystem`, `BehaviorSystem`) update internal states of all entities (NPCs).  
3. **StoryManager** (OOP) communicates with the LLM backend for emergent dialogue/events and updates the ECS with new narrative states (e.g., new objectives, quests, or plot twists).

---

# 5. **AI Systems**

## 5.1 Traditional AI

1. **Behavior Trees / State Machines**  
   - Used for straightforward NPC tasks (patrols, resource management, daily routine).  
   - Example: A servant who cycles through daily tasks—cleaning, serving, resting.  

2. **Pathfinding**  
   - Determines how an NPC navigates the 3D environment (e.g., corridors, throne room).  
   - Built on ECS data for location and movement components.

## 5.2 LLM-Driven AI

1. **Dialogue & Interpersonal Dynamics**  
   - NPCs engage in conversations with each other or the player using an LLM.  
   - Context includes social status, past interactions, or alliances.  
   - Dialogue can generate new story branches (e.g., conspiracies, confessions).

2. **Narrative Generation**  
   - The LLM proposes event chains or plot hooks based on the current state of the court.  
   - Example: If the King is rumored to be ill, the LLM might generate new claimants, foreign emissaries looking to exploit the situation, etc.

3. **Adaptive Behavior**  
   - When the player takes an unexpected action, the LLM can adapt the story or NPC behaviors on the fly.  
   - This ensures no two playthroughs are identical.

---

# 6. **Core Subsystems**

1. **Social/Relations System**  
   - Tracks friendships, enmities, loyalties, and alliances.  
   - Each relationship can have multiple metrics (e.g., trust, fear, respect).

2. **Reputation & Influence System**  
   - Governs how NPCs react to the player based on their deeds and standing.  
   - Gains or losses in influence can impact interactions, unlock new storylines, or restrict access to certain court areas.

3. **Economy & Resource System**  
   - Simulates the flow of resources (e.g., gold, food, land ownership).  
   - Vital to roles like Merchant or Steward who rely on economic power.

4. **Political System**  
   - Models power structures in the court (King, Queen, Council, Nobles, etc.).  
   - Tracks official titles, roles, and privileges.  
   - Feeds into the LLM to generate relevant storyline events (e.g., “A new Duke demands more land,” or “Council proposes a new tax.”).

5. **Event/Quest System**  
   - Semi-randomly spawns events (festivals, crises, foreign visits) or personal quests (duels, negotiations, espionage).  
   - Integrates with LLM for dynamically generated plot twists or expansions.

---

# 7. **User Interface & Experience**

1. **Role-Based UI**  
   - Customizable panels based on player’s role. (e.g., an Overseer sees economic dashboards, a Knight sees combat readiness and personal renown).  

2. **Dialogue Window**  
   - Displays emergent conversations with multiple branching choices.  
   - Integrates with the LLM for on-the-fly generated responses.

3. **Court Map & Character Profiles**  
   - Provides an overview of character relations and real-time status updates.  
   - The player can inspect alliances or enmities and read short AI-generated summaries.

4. **Notifications & Logs**  
   - Summaries of relevant story events (e.g., “Lord A allied with Lord B,” “You lost 10 Influence,” etc.).  
   - Log of dialogues and significant plot developments for player reference.

---

# 8. **Scalability & Performance Considerations**

1. **Efficient ECS Updates**  
   - Group NPCs in “zones” or “batches” for updates.  
   - Systems (e.g., AIBehaviorSystem) only process entities within active or high-priority areas.

2. **LLM Query Management**  
   - Cache repeated requests and maintain context windows to prevent redundant calls.  
   - Implement “summary states” for NPC relationships so the LLM doesn’t re-derive the same background each time.

3. **Network/Multiplayer (Future Consideration)**  
   - Potentially expand to co-op or competitive online experiences.  
   - Leverage a client-server ECS approach for sync.

---

# 9. **Modularity & Extensibility**

1. **Component-Based Features**  
   - Adding new features (e.g., a new type of NPC or a new economic resource) primarily requires adding or extending relevant components and systems.

2. **Plugins/Expansion Packs**  
   - New storyline expansions can be dropped in via the **StoryManager**.  
   - Additional AI modules or specialized roles can be introduced with minimal codebase disruption.

3. **Data-Driven Configuration**  
   - Use external JSON/XML config files for balancing or modding.  
   - The LLM can draw from user-provided lore expansions if desired.

---

# 10. **Development Roadmap**

1. **Pre-Alpha** (Core Systems & Proof-of-Concept)
   - Establish the ECS framework and basic OOP management classes.  
   - Integrate minimal LLM functionality for simple dialogues.

2. **Alpha** (Core Gameplay & AI)
   - Implement full set of ECS Systems (Behavior, Relation, Economy).  
   - Expand LLM dialogue with branching conversation logic.  
   - Introduce the social/political systems.

3. **Beta** (Content & Polish)
   - Add diverse roles and quests.  
   - Focus on UI/UX improvements and performance optimizations.  
   - Build out emergent event scenarios and test replayability.

4. **Release** (Balancing & Finalization)
   - Final pass on balancing, bug fixes, and performance.  
   - Integration of modding tools (if applicable).  
   - Polish dialogues, art, and user feedback loops.

---

# 11. **Conclusion**

This specification outlines a **hybrid ECS/OOP** architecture leveraging both **traditional AI** and an **LLM** to drive **emergent storytelling** in a medieval royal court. By focusing on **replayability**, **scalability**, and **modularity**, the game aims to provide endlessly varied, dynamic story arcs that encourage players to experiment with different roles and strategic approaches.

If executed properly, *Courtly Intrigues* (or any final title chosen) will deliver a deeply immersive, ever-evolving narrative experience that sets a new standard for AI-driven emergent gameplay.

---