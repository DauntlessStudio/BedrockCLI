# Commands
- [Commands](#commands)
    - [Comp](#comp)
    - [Cogr](#cogr)
    - [Dmgs](#dmgs)
    - [Eanim](#eanim)
    - [Prop](#prop)
    - [Eprop](#eprop)
    - [Pent](#pent)
    - [Nent](#nent)
    - [Nitm](#nitm)
    - [Nblk](#nblk)
    - [Anim](#anim)
    - [Ctrl](#ctrl)
    - [Func](#func)
    - [Sdef](#sdef)
    - [Pkg](#pkg)

## Comp
Adds or removes components to the entities.
```
Usage:
  comp [OPTION...]

  -h, --help           View help
  -c, --component arg  Component to add in JSON format, or the name of the component to remove
  -i, --indent arg     JSON file indent (default: 4)
  -f, --family arg     Family types to modify
  -d, --directory arg  Subdirectory to modify
  -r, --remove         Remove component
  -n, --name arg       Filenames of entities to modify, i.e. player.json
```
Example
```
bed comp --component "{\\"minecraft:is_sheared\\":{}}" --family mob
```
This command will add the "minecraft:is_sheared" component to every entity in the project that contains the family type "mob".

## Cogr
Adds or removes component groups to the entities.
```
Usage:
  cogr [OPTION...]

  -h, --help           View help
  -g, --group arg      The component group to add/remove, either a name or JSON string
  -i, --indent arg     JSON file indent (default: 4)
  -f, --family arg     Family types to modify
  -d, --directory arg  Subdirectory to modify
  -r, --remove         Remove group
  -n, --name arg       Filenames of entities to modify, i.e. player.json
```
Example
```
bed cogr --group "{\\"custom_group\\":{\\"minecraft:is_sheared\\":{}}}" --family mob
```
This command will add a component group called "custom_group" containing the "minecraft:is_sheared" component to every entity in the project that contains the family type "mob".

This also will attach events called "add_custom_group" and "remove_custom_group" that will add and remove the new component groups respectively.

## Dmgs
Adds or removes damage sensor entries to the entities.
```
Usage:
  dmgs [OPTION...]

  -h, --help           View help
  -s, --sensor arg     The damage sensor to add/remove, either a name or JSON string
  -i, --indent arg     JSON file indent (default: 4)
  -f, --family arg     Family types to modify
  -d, --directory arg  Subdirectory to modify
  -n, --name arg       Filenames of entities to modify, i.e. player.json
```
Example
```
bed dmgs --sensor "{ \"on_damage\": { \"event\": \"add_custom_group\", \"target\": \"self\", \"filters\": [ { \"test\": \"is_family\", \"subject\": \"other\", \"value\": \"player\" } }" --family mob
```
This command will add a damage sensor entry which runs the event "add_custom_group" when the entity is hit by a mob with the "player" family type to every entity in the project that contains the family type "mob".

This sensor entry will be the last entry in all damage sensors.

## Eanim
Adds or removes animation references to the entities.
```
Usage:
  eanim [OPTION...]

  -h, --help           View help
  -a, --animation arg  Animation names to add/remove as 'entity.animation'
  -s, --script         Add animation to scripts
  -c, --controller     Is animation a controller
  -i, --indent arg     JSON file indent (default: 4)
  -f, --family arg     Family types to modify
  -d, --directory arg  Subdirectory to modify
  -r, --remove         Remove animation
  -n, --name arg       Filenames of entities to modify
```
Example
```
bed eanim --animation generic.custom_anim --script --family mob
```
This command will add an animation entry called "animation.generic.custom_anim" to every entity in the project that contains the family type "mob".

With the `--script` argument it will also add it to `minecraft:entity>description>scripts>animate`.

## Prop
Adds properties to the entities.
```
Usage:
  prop [OPTION...]

  -h, --help           View help
  -p, --property arg   Property to add as 'namespace:property'
  -i, --indent arg     JSON file indent (default: 4)
  -f, --family arg     Family types to modify
  -d, --directory arg  Subdirectory to modify
  -t, --type arg       Property type, valid values are: bool, enum, float,
                       int (default: bool)
  -v, --values arg     Possible values, seperated by commas
      --default arg    Default value
  -s, --sync           Client sync
  -n, --name arg       Filenames of entities to modify
```
Example
```
bed prop --property ldz:custom_property --type int --values 0,5 --default 1 --family mob
```
This command will add a new property called "ldz:custom_property" of type `int` with a range of [0, 5], and a default value of 1 to every entity in the project that contains the family type "mob".

Example 2
```
bed prop --property ldz:custom_property --type enum --values north,south,east,west --default north --family mob
```
This command will add a new property called "ldz:custom_property" of type `enum` with acceptable values of `north`, `south`, `east`, `west`, and a default value of `north` to every entity in the project that contains the family type "mob".

## Eprop
Adds property event to entities.
```
Usage:
  eprop [OPTION...]

  -h, --help            View help
  -p, --property arg    Property to add as 'namespace:property'
  -i, --indent arg      JSON file indent (default: 4)
  -f, --family arg      Family types to modify
  -d, --directory arg   Subdirectory to modify
  -v, --value arg       New value
  -e, --event_name arg  Name of the event. Defaults to
                        'set_(property)_to_(value)
  -n, --name arg        Filenames of entities to modify
```
Example
```
bed eprop --property ldz:custom_property --value 5 --family mob
```
This command will add a new event called "set_custom_property_to_5" that will set "ldz:custom_property" to 5 to every entity in the project that contains the family type "mob".

## Pent
Adds player entity.
```
Usage:
  pent [OPTION...]

  -h, --help        View help
  -b, --bp          Create bp entry for entity
  -r, --rp          Create rp entry for entity
  -g, --geo         Initialize for 3D geos
  -i, --indent arg  JSON file indent (default: 4)
```
Example
```
bed pent --bp --rp --geo
```
This command will import the BP player.json file, the RP player.entity.json file, and initialize the player.entity.json to have all the required setup to allow for custom geo weapons.

## Nent
Adds new entities
```
Usage:
  nent [OPTION...]

  -h, --help                 View help
  -l, --lang [=arg(=en_US)]  Lang files to add entries to
  -e, --enemy                Create hostile enemy
  -p, --passive              Create passive mob
  -m, --model                Create new model with entity
  -r, --rp                   Create rp entry for entity
  -i, --indent arg           JSON file indent (default: 4)
  -n, --name arg             Entity names to add
```
Example
```
bed nent --lang --passive --model --rp --name ldz:custom_entity
```
This command will create a new entity with passive mob behaviors, a generic cube model, an entity.json file, and a lang file entry for "ldz:custom_entity".

Not using --passive or --hostile will default to a dummy entity.

## Nitm
Adds new items.
```
Usage:
  nitm [OPTION...]

  -h, --help                 View help
  -l, --lang [=arg(=en_US)]  Lang files to add entries to
  -s, --stack arg            Maximum stack size (default: 64)
  -i, --indent arg           JSON file indent (default: 4)
  -e, --edible               Is edible
  -a, --attachable           Create attachable
  -n, --name arg             Item names to add
```
Example
```
bed nitm --lang --stack 1 --attachable --name ldz:custom_item
```
This command will create a new item with a max stack size of 1, an attachable with a custom model, a sprite reference, a lang file entry, and will modify the player.entity.json, player animation controller, and player animations to allow for the custom items.

When using --attachable it would make sense to run `bed pent --rp --geo` to setup the player entity for custom geo usage first.

Not using --attachable will create a generic sprite item.

## Nblk
Adds new blocks.
```
Usage:
  nblk [OPTION...]

  -h, --help                 View help
  -t, --table                Add loot table
  -l, --lang [=arg(=en_US)]  Lang files to add entries to
  -i, --indent arg           JSON file indent (default: 4)
  -e, --emissive arg         Block emits light (default: 0.0, maximum 1.0)
  -n, --name arg             Block names to add
```
Example
```
bed nblk --lang --table --name ldz:custom_block
```
This command will create a new block and a custom loot table that drops the newly created block. This also adds the new block to the terrain_texture.json file.

## Anim
Adds new animation.
```
Usage:
  anim [OPTION...]

  -h, --help          View help
  -l, --loop          Should animation loop
  -i, --indent arg    JSON file indent (default: 4)
  -c, --commands arg  Commands to play (default: /say test)
  -t, --time arg      The length of the animation (default: 1.0)
  -n, --name arg      Animation name to add as 'entity.anim'
```
Example
```
bed anim --loop --commands "/say command 1,/say command 2" --time 10.0 --name generic.custom_anim
```
This command will create a new looping animation. At the timeline entry [0.0] it will run the commands `say command 1`, `say command 2`. The animation loops every 10.0 seconds.

## Ctrl
Adds new animation controller.
```
Usage:
  ctrl [OPTION...]

  -h, --help           View help
  -i, --indent arg     JSON file indent (default: 4)
      --entry arg      The on_entry values
      --exit arg       The on_exit values
  -a, --animation arg  The animation values
      --return arg     The query to transition back to default
  -q, --query arg      The query to transition from default (default: true)
  -s, --state arg      The name of the state to transition to (default:
                       state_1)
  -n, --name arg       Controller name to add as 'entity.anim'
```
Example
```
bed ctrl --entry "/say on_entry" --exit "/say on_exit" --animation custom_animation --query query.property('ldz:custom_property') --return true --name generic.controller
```
This command will create a new animation controller like this:
```json
{
    "controller.animation.generic.controller": {
        "initial_state": "default",
        "states": {
            "default": {
                "transitions": [
                    { "state_1": "query.property('ldz:custom_property')" }
                ]
            },
            "state_1": {
                "on_entry": [
                    "/say on_entry"
                ],
                "animations": [
                    "custom_animation"
                ],
                "transitions": [
                    { "default": "true" }
                ],
                "on_exit": [
                    "/say on_exit"
                ]
            }
        }
    }
}
```

## Func
Adds new function.
```
Usage:
  func [OPTION...]

  -h, --help          View help
  -c, --commands arg  The commands, using ';' as a delimiter (default: say test)
  -n, --name arg      Function names `foo/bar`
```
Example
```
bed func --commands "say Hello World!" --name foo/bar
```
This command will create a function at functions/foo/bar.mcfunction with the command `say Hello World`.

## Sdef
Adds new sound definition.
```
Usage:
  sdef [OPTION...]

  -h, --help            View help
  -i, --indent arg      JSON file indent (default: 4)
  -v, --volume arg      Volume for sounds (default: 1.0)
  -p, --pitch arg       Pitch for sounds (default: 1.0)
  -c, --category arg    Category of definition (default: neutral)
  -f, --filepath arg    Paths to sound files
  -d, --definition arg  Name of vanilla definition to copy into new
                        definition
  -n, --name arg        Name of definitions
```
Example
```
bed sdef --definition mob.allay.death --name custom.allay.death
```
This command will create a new entry in sound_definitions.json. The --definition argument will attempt to find a vanilla sound called "mob.allay.death" and insert a copy of that sound called "custom.allay.death".

## Pkg
A simple package manager.
```
Usage:
  pkg [OPTION...]

  -h, --help        View help
  -l, --list        Lists available packages
      --import arg  Import package using either name or index
  -i, --indent arg  JSON file indent (default: 4)
```
Example
```
bed pkg --list
```
This command will list available packages, i.e.
* [0] "Animated Texture Demo"
* [1] "Bob Arrow"
* [2] "Custom Actionbar"
* [3] "Exit Sign"
* [4] "Ground Slam Particle"
* [5] "Health Bar"
* [6] "Outline Template"
* [7] "Scoreboard Teleporter"
* [8] "Spawn Effect Particle"
* [9] "Speech Bubble Particle"

Example 2
```
bed pkg --import 1
```
or
```
bed pkg --import "Bob Arrow"
```
This command will import the Bob Arrow package into your project.

This requires an environment variable on your PC called GITHUB_TOKEN that has a valid GitHub OAuth token.

Packages are hosted [here](https://github.com/Dotzip-Developments/DotzipPackages).