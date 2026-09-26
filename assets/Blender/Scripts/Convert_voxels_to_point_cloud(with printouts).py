import bpy
import time

#Function to print out the material values for each object
def get_all_material_values():
    obs = bpy.data.collections['MATERIALS'].objects
    for ob in obs:
        col = ob.material_slots[0].material.diffuse_color
        print(ob.name, list(col))

#helper function to replace . with _ on material names
def rename_point_to_underscore():
    objs = bpy.context.selected_objects
    for obj in objs:
        obj.name = obj.name.replace('.', '_')

# Safer way to toggle undo
def set_undo(value):
    if hasattr(bpy.context.preferences.edit, "use_undo"):
        bpy.context.preferences.edit.use_undo = value
    elif hasattr(bpy.context.preferences.edit, "use_global_undo"):
        bpy.context.preferences.edit.use_global_undo = value

#function to ensure old bone constraint data is not intefering
def cleanup_armature_ghosts():
    obj = bpy.context.object
    obj.update_tag()
    bpy.context.view_layer.update()

#seoerate objects
def seperate_objects():
     
     # 1. Select the object
    obj = bpy.context.active_object
    
    # 2. Switch to Edit Mode (Required for the separate operator)
    bpy.ops.object.mode_set(mode='EDIT')
    
    # 3. Select all geometry inside the mesh
    bpy.ops.mesh.select_all(action='SELECT')
    
    # 4. Separate by loose parts
    bpy.ops.mesh.separate(type='LOOSE')
    
    # 5. Switch back to Object Mode
    bpy.ops.object.mode_set(mode='OBJECT')
    
    print(f"Separation complete. New objects created.")

# sets origin of all selected objects to the centre of the volume
def set_origin_to_volume():
    bpy.context.scene.tool_settings.transform_pivot_point = 'INDIVIDUAL_ORIGINS'
    bpy.ops.object.origin_set(type='ORIGIN_GEOMETRY', center='MEDIAN')

#scales all objecst to 0 and then applys the scale
def scale_objects():
    objs = bpy.context.selected_objects
    for obj in objs:
        obj.scale = (0.0, 0.0, 0.0)
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)

#function to remove doubles
def remove_doubles():
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.remove_doubles(threshold=0.001)
    bpy.ops.object.mode_set(mode='OBJECT')

#Function to join all object and remove doubles
def join_objects():
    # 1. Ensure we are in Object Mode
    if bpy.context.mode != 'OBJECT':
        bpy.ops.object.mode_set(mode='OBJECT')
        
    targets = [obj for obj in bpy.context.selected_objects if obj.type == 'MESH']

    if len(targets) > 1:
        
        bpy.context.view_layer.objects.active = targets[0]
         
        for obj in targets:
            obj.select_set(True)
            
        bpy.ops.object.join()
        
        remove_doubles()
        print(f"Successfully joined {len(targets)} objects into {targets[0].name}")
    else:
        print("Join skipped: Not enough objects selected.")

#joining function that should be more efficent speed and memory wise
def batch_join_with_data(obs, chunk_size=500):
   
    if not obs:
        return
    
    # 1. Ensure we are in Object Mode
    if bpy.context.mode != 'OBJECT':
        bpy.ops.object.mode_set(mode='EDIT') # Some users report this helps clear buffers
        bpy.ops.object.mode_set(mode='OBJECT')

    # Store the intermediate joined objects
    intermediate_results = []
    
    # Copy the list so we don't mutate the one we are iterating over
    to_join = obs.copy()
    
    print(f"Starting Batch Join of {len(to_join)} objects...")

    while len(to_join) > 0:
        # Pull out a chunk
        chunk   = to_join[:chunk_size]
        to_join = to_join[chunk_size:]
        
        # Select the chunk
        bpy.ops.object.select_all(action='DESELECT')
        for o in chunk:
            o.select_set(True)
        
        # Set active to the first in chunk (it will inherit the weights/mats)
        bpy.context.view_layer.objects.active = chunk[0]
        
        # Join this batch
        bpy.ops.object.join()
        
        # Save the result for the final round
        intermediate_results.append(bpy.context.active_object)
        print(f"Finished batch. {len(to_join)} objects remaining...")

    # 2. Final Round: Join the intermediate results together
    if len(intermediate_results) > 1:
        bpy.ops.object.select_all(action='DESELECT')
        for o in intermediate_results:
            o.select_set(True)
        bpy.context.view_layer.objects.active = intermediate_results[0]
        bpy.ops.object.join()
    
    remove_doubles()
    print("All objects joined successfully while preserving weights and materials.")

# function to convert the voxels to single points
def convert_to_point_cloud():
    
    #saftey check for if there is an active object or not
    if not bpy.context.active_object:
        print("Error: No active object selected.")
        return
    
    #clean up armature data
    cleanup_armature_ghosts()
    
    set_undo(False)
    
    #bpy.context.preferences.edit.use_undo = False
    #call to seperate objecst
    seperate_objects()
    
    #set origin to volume
    set_origin_to_volume()
    
    #scale to 0 and apply scale
    scale_objects()
    
    #join all selected objects
    #join_objects()
    
    targets = [obj for obj in bpy.context.selected_objects if obj.type == 'MESH']
    batch_join_with_data(targets, 1000)
    
    set_undo(True)
    #bpy.context.preferences.edit.use_undo = True
    print("mesh converted to point cloud succesfully")
    
    
# Start the timer
start_time = time.perf_counter()

#run conversion to point cloud
convert_to_point_cloud()  

# Calculate duration
end_time = time.perf_counter()
total_time = end_time - start_time
        
print("-" * 30)
print(f"PROCESS COMPLETED")
print(f"Total Time: {total_time:.4f} seconds")
print("-" * 30)
   
