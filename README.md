# Bedrock CLI

A command line tool to help develop Minecraft Bedrock addons.

## Description

This program provides useful tools to quickly perfom laborious or repetitive tasks when making a Bedrock addon. Focus was especially placed on the ability to make batch changes. The small footprint and simple command line interface allows it to be easily used by other programs as well.

## Getting Started

### Dependencies

* Windows 10 or higher

### Installing

* Download the .zip from the [Latest Release](https://github.com/DauntlessStudio/BedrockCLI/releases/latest)
* Extract the .zip anywhere on your machine (i.e. Downloads/BedrockCLI)
* Right Click the install.bat file and `Run as administrator`

### Executing program

* If working with projects where your behavior pack and resource pack are contained in a `world_template`, then you can run the program from any directory in your `world_template` and it will use the first folders in `world_template/behavior_packs` and `world_template/resource_packs`.
* If run outside of a `world_template` it will use the first folders in your `com.mojang/development_behavior_packs` and `com.mojang/development_resource_packs`.
* If you want to target specific folders rather that let the program automatically infer them, you can provide a filepath with the --bpath and --rpath args.
```
  bed [OPTION...] <command>

  -h, --help       View help
      --bpath arg  The path to the behavior pack root
      --rpath arg  The path to the resource pack root
```

## Help

For a list of commands use:
```
bed -h, --help
```
For help with a specific command use:
```
bed <command> -h, --help
```
### Command List
You can find specific documentation for all commands [here](documentation/commands.md)

## Authors

Contributors names and contact info

[@Dauntless Studio](https://github.com//dauntlessstudio)

## Version History

* 0.1.0
    * Initial Release
* 0.1.1
    * Added PROP Command
    * Added EPROP Command
    * Bug Fixes
* 0.1.2
    * Organized Folder Structure
    * Optimizations
    * Bug Fixes
* 0.1.3
    * Added DMGS Command
    * Bug Fixes
* 0.1.4
    * Bug Fixes
* 0.1.5
    * Added PKG Command
    * Added --Attachable Arg to NITM
    * Added PENT Command
    * Added Version Checking
