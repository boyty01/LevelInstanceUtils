Simple system to handle making runtime changes to specific level instance actors.  

# Usage

1. Add a SubLevelActorScriptComponent to every actor in your Level Instance that you want to run custom behavior.

2. Set the name of the manager class that this actor should request its data from at runtime.
![image](https://github.com/user-attachments/assets/ea4193ab-b9a0-427b-84f9-550465adfb3e)

3. Add a (or subclass of) ASubLevelActorManagerBase to the persistent map that your level instances are sublevels of.
 
4. Set the Manager Name value to the same value you set in your script components.

5. To simplify identifying the actors you want to manage, you can use the editor function to search a specified level instance for every actor that has a script component whose manager name matches this manager. Every match will automatically be added as an entry in the managers Guid map.

![image](https://github.com/user-attachments/assets/4ad8484e-4c48-451f-b62f-cef0fa6528cb)

6. Subclass SubLevelActorScriptBase and override the Execute function with the logic you want to perform on the actor.
  
![image](https://github.com/user-attachments/assets/98bae642-994a-4abe-b775-ecdfd7c52925)


7. Assign the script to the appropriate guid in the managers map.

# extra context 
If a client requests a script from the subsystem for a manager that isn't registered, the request is queued and dispatched as soon as the manager becomes available. This replaces the old approach of a component retrying after a specified delay, resulting in guaranteed near-instant script execution as soon as possible, at any point in the world life cycle.

