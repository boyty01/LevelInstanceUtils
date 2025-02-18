Simple system to handle making runtime changes to specific level instance actors.  

# Usage

1. Add a SubLevelActorScriptComponent to every actor in your Level Instance that you want to run custom behavior.

2. Set the name of the manager class that this actor should request its data from at runtime.
![image](https://github.com/user-attachments/assets/ea4193ab-b9a0-427b-84f9-550465adfb3e)

3. Add a (or subclass of) ASubLevelActorManagerBase to the persistent map that your level instances are sublevels of.
 
4. Set the Manager Name value to the same value you set in your script components.

5. To simplify identifying the actors you want to manage, you can use the editor function ClaimGuidsFromlevel() to search a specified level instance for every actor that has a script component whose manager name matches this manager. Every match will automatically be added as an entry in the managers Guid map. This is the recommended way of adding actors to the manager's list as it gathers additional data that helps identify the actor, specifically used for ApplyInEditor() for previewing scripts at in the editor. 

![image](https://github.com/user-attachments/assets/36621605-480d-4c8e-8066-450a144deda6)


6. Subclass SubLevelActorScriptBase and override the Execute function with the logic you want to perform on the actor.
  
![image](https://github.com/user-attachments/assets/98bae642-994a-4abe-b775-ecdfd7c52925)


7. Assign the script to the appropriate guid in the managers map.

# Previewing Scripts
Because of the way level instances work, you can't apply scripts at design time and have the changes persist when saving the map. But you can apply the scripts in editor to make sure they perform the action you expect them to. You can do this by calling ApplyInEditor(). 

# extra context 

## Watchouts 
As part of the data gathering that happens with ClaimGuidsFromLevel(), information such as the actors Label and the Level Instance Actor Label are stored for both allowing ApplyInEditor() to reverse search for the actor and allow designers to identify the actor easier. This information is only gathered when the function is executed in the editor, and any changes made to the actor labels will not automatically be reflected. If you change either the level instance or the actors labels after they're added to the list, you'll need to run ClaimGuidsFromLevel() again to update the information - Doing so will not reset the User Friendly name nor the Script class. 

## Built-in Race Condition Resolution
If a client requests a script from the subsystem for a manager that isn't registered, the request is queued and dispatched as soon as the manager becomes available. This replaces the old approach of a component retrying after a specified delay, resulting in guaranteed near-instant script execution as soon as possible, at any point in the world life cycle.

## Hard references to script Classes in the manager
The original idea was to Asynchronously load script classes as soon as they were requested, to avoid any unnecessary memory hogging before they're actually needed in a world partition map - naturally the player could be wandering in an area that didn't ever load an actor that needed a specific script. After some deliberation though, it felt as though the Script classes were typically lightweight enough to be negligible on memory footprint until a pretty unreasonable number of scripts were added to a single map. Any lazy loaded hard class references would be almost immediately dropped out of scope once they're loaded, and due to the way unreals garbage collection works they'd be removed from memory again almost instantly, which ultimately would lead us to situations where assets are just continuously being loaded and unloaded. So for now I've decided that Managers keep hard references to script classes for both the sake of simple, readable code and for more predictable game performance. In addition to this, the Script objects are never instantiated and the logic is only ever Executed using the CDO for speed and optimisation. This combination of preloading the CDO and no instantiation results in the fastest possible execution of the scripts to reduce any unwanted performance or quality problems.

