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
Script components have  retry time and max retries properties to to resolve race conditions when the world is first brought up. Since managers register themselves on  begin play, it's quite possible that a script component might try to access it's manager before the subsystem has stored a reference. The subsystem is hard coded to reject any client (script component) request for their managers for the first 1 second of game time to allow managers time to register. If a script components query is rejected, it will try again in {RetryTime} seconds for {MaxRetries} attempts before giving up. The delay in the client retrieving and executing it's scripts may cause a brief period at the start of play where the client is visibly in it's default state. You can tweak these values to balance mitigating this issue with the race conditions for a smoother experience. For level instances that load later than the worlds initial begin play there should be very little delay before scripts execute. 

