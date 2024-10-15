# Dragon's Fury

**Dragon's Fury** is C++-based TPS shooting game where players must defeat a powerful Dragon by destroying protective crystals and surviving its attacks.

## Features

### 1. Player Abilities
- **Gun Equipped**: Players start the game equipped with a gun to engage enemies.
- **Flying Mechanic**: Press the `Shift` key to fly and chase the Dragon in the sky. This ability has a cooldown indicated by the **Fly Icon**.

### 2. Enemy: The Dragon
- **Dragon Attacks**: The Dragon can fly and attack the player. Contact with the Dragon deals significant damage, so dodging is critical.
- **Crystal Protection**: The Dragon is invulnerable until the player destroys 10 Crystals. After the Crystals are destroyed, the Dragon becomes vulnerable.
- **Game Clear**: Once all Crystals are destroyed and the Dragon is defeated, the game is won.

### 3. Game Structure
- **Game Modes**: The game transitions through different states such as Ready, Start, Playing, and End.
- **Weapon Component**: Weapons are reusable components available to any character inheriting from the `ACharacter` class.
- **Player and Enemy FSM**: Both the Player and the Dragon use finite state machines (`FSM`) to manage behaviors like Idle, Move, Fly, and Attack.

## How to Play
1. Destroy all 10 Crystals to make the Dragon vulnerable.
2. Dodge the Dragon's attacks and use your gun to deal damage.
3. Fly using the **Shift** key to chase the Dragon. Be mindful of the flight cooldown.
4. Defeat the Dragon to win the game!
