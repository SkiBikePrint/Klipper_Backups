# Include servo hardware definition separately to allow for automatic upgrade
[include blobifier_hw.cfg]

##########################################################################################

# Sample config to be used in conjunction with Blobifier Purge Tray, Bucket & Nozzle 
# Scrubber mod. Created by Dendrowen (dendrowen on Discord). The Macro is based on a 
# version, and Nozzle Scrubber is made by Hernsl (hernsl#8860 on Discord). The device is 
# designed around a Voron V2.4 300mm, but should work for 250mm and 350mm too. This 
# version only supports the assembly on the rear-left of the bed. If you decide to change 
# that, please consider contributing to the project by creating a pull request with the 
# needed changes.

# IMPORTANT: The rear-left part of your bed becomes unusable by this mod because the 
#            toolhead needs to lower down to 0. Be sure not to use the left-rear 130x35mm.

# The goals of this combination of devices is to dispose of purged filament during a 
# multicolored print without the need of a purge block and without the flurries of
# filament poops consuming your entire 3D printer room. The Blobifier achieves that by
# purging onto a retractable tray which causes the filament to turn into a tiny blob 
# rather then a large spiral. This keeps the waste relativly small. The bucket should be
# able to account for up to 200 filament swaps (for the 300mm V2).

# Upload this file to your klipper config directory and include it in printer.cfg
# [include mmu/addons/blobifier.cfg]

# It is advised to use the start_gcode from Happy Hare. Then you will be able to fully 
# and efficiently use this mod. Check the Happy Hare document at gcode_preprocessing.md 
# in the Happy Hare github for more details.

###################################### DISCLAIMER ########################################

# You, and you alone, are responsible for the correct execution of these macros and 
# gcodes. Any damage that may occur to your machine remains your responsibility. 
# Especially when executing this macro for the first few times, keep an eye on your 
# printer and the 
# emergency stop.

##########################################################################################

##########################################################################################
# Main macro. Usually you should only need to call this one or place it in the Happy Hare
# _MMU_POST_LOAD macro or variable_user_post_load_extension:
# PURGE_LENGTH=[float] (optional) The length to purge. If ommited it will check the 
#                      purge_volumes matrix or variable_purge_length
# PURGE_RET=[float]    (optional) The length to retract between moves
#
[gcode_macro BLOBIFIER]
# These parameters define your filament purging. The retract variable is used to retract 
# right after purging to prevent unnecessary oozing. Some filament are particularly oozy 
# and may continue to ooze out of the nozzle for a second or two after retracting. Update 
# as necessary.
variable_purge_spd: 400	     # Speed, in mm/min, of the purge.
variable_purge_temp_min: 240	     # Minimum nozzle purge temperature.
variable_retract_before_park: 0.0     # amount in mm. 0 to disable
variable_retract_speed: 2000       # mm/m

# Adjust this so that your nozzle scrubs within the brush. Be careful not to go too low! 
# Start out with a high value (like, 6) and go 
# down from there.
variable_brush_top: 6

# These parameters define your scrubbing, travel speeds, safe z clearance and how many 
# times you want to wipe. Update as necessary.
variable_clearance_z: 2	         # When traveling, but not cleaning, the 
                                           #   clearance along the z-axis between nozzle 
                                           #   and brush.
variable_wipe_qty: 2	         # Number of complete (A complete wipe: left, 
                                           #   right, left OR right, left, right) wipes.
variable_travel_spd_xy: 10000	         # Travel (not cleaning) speed along x and 
                                           #   y-axis in mm/min.
variable_travel_spd_z: 1000	         # Travel (not cleaning) speed along z axis 
                                           #   in mm/min.
variable_wipe_spd_xy: 10000	         # Nozzle wipe speed in mm/min.


# These parameters define the size of the brush. Update as necessary. A visual reference 
# is provided below.
# 
#                  ←   brush_width   →                            
#                   _________________                                                          
#                  |                 |  ↑                Y position is acquired from your 
#  brush_start (x) |                 | brush_depth       stepper_y position_max. Adjust 
#                  |_________________|  ↓                your brush physically in Y so 
#                          (y)                           that the nozzle scrubs within the 
#                      brush_front                       brush.                                              
# __________________________________________________________
#                     PRINTER FRONT
# 
# 
# Start location of the brush. Defaults for 250, 300 and 350mm are provided below. 
# Uncomment as necessary
#variable_brush_start:          34	# For 250mm build    
variable_brush_start: 7  # For 300mm build
#variable_brush_start:          84  # for 350mm build

# width of the brush
variable_brush_width: 35

# Location of where to purge. The tray is 15mm in length, so if you assemble it against 
# the side of the bed (default), 10mm is a good location
variable_purge_x: 7

# Height of the tray. If it's below your bed, give this a negative number equal to the 
# difference. If it's above your bed, give it a positive number. You can find this number 
# by homing, optional QGL or equivalent, and moving you toolhead above the tray, and 
# lowering it with the paper method. 
variable_tray_top: 0.0

# Servo angles for tray positions
variable_tray_angle_out: 0
variable_tray_angle_in: 180

# ========================================================================================
# ==================== BLOB TUNING =======================================================
# ========================================================================================

# The following section defines how the purging sequence is executed. This is where you 
# tune the purging to create pretty blobs. Refer to the visual reference for a better 
# understanding. The visual is populated with example values. Below are some guides 
# provided to help with tuning.
#
#                          \_____________/
#                             |___|___|
#                                \_/            ______________  < End of third iteration.
#                                / \                                  HEIGHT:   3 x iteration_z_raise - (2 + 1) x iteration_z_change  (3 x 5 - 2 x 1.2 = 11.4)
#                               |   |                                 EXTRUDED: 3 x max_iteration_length                              (3 x 50 = 150)
#                              /     \          ______________  < End of second iteration.
#                             |       \                               HEIGHT:   2 x iteration_z_raise - 1 x iteration_z_change        (2 x 5 - 1 x 1.2 = 8.8)
#                            /         |                              EXTRUDED: 2 x max_iteration_length                              (2 x 50 = 100)
#                           |           \       ______________  < End of first iteration. 
#                          /             \                            HEIGHT:   1 x iteration_z_raise                                 (1 x 5 = 5)
#                         |               |                           EXTRUDED: 1 x max_iteration_length                              (1 x 50 = 50)
#___________               \             /      ______________  < Start height of the nozzle. default value: 1.5mm
#           |_______________\___________/_      ______________  < Bottom of the tray
#           |_____________________________|
#           |
# 
########################### BLOB TUNING ##############################
# +-------------------------------------+----------------------------+
# |  Filament sticks to the nozzle at   | Incr. purge start          |
# |    initial purge (first few mm)     |                            |
# +-------------------------------------+----------------------------+
# |  Filament scoots out from under     | Incr. temperature          |
# |  the nozzle at the first iteration  | Decr. iteration_z_raise    |
# |                                     | Incr. max_iteration_length |
# +-------------------------------------+----------------------------+
# |  Filament scoots out from under the | Decr. purge_spd            |
# |  the nozzle at later iterations     | Incr. iteration_z_change   |
# +-------------------------------------+----------------------------+
# |  Filament sticks to the nozzle at   | Decr. iteration_z_change   |
# |         later iterations            |                            |
# +-------------------------------------+----------------------------+
# |        I think my blobs can         | Balsy... Increase          |
# |          be much bigger!            | max_iterations_per_blob    |
# +-------------------------------------+----------------------------+
#

# The height to raise the nozzle above the tray before purging. This allows any built up 
# pressure to escape before the purge.
variable_purge_start: 0.5

# The maximum mm of filament (length) to extrude one iteration. A blob contains multiple 
# iterations
variable_max_iteration_length: 50

# The amount to raise Z for each iteration
variable_iteration_z_raise: 6

# As the nozzle gets higher and the blob wider, the Z raise needs to be reduced.
variable_iteration_z_change: .9

# Maximum iterations per blob. 3 Should be sufficient for most flushes. If not, Blobifier 
# will create multiple blobs for a single purge.
variable_max_iterations_per_blob: 3

# Lift the nozzle slightly after creating the blob te release pressure on the tray.
variable_eject_hop: 0.0

# Dwell time (ms) after purging and before cleaning to relieve pressure from the nozzle.
variable_pressure_release_time: 0

# Set the part cooling fan speed. Disabling can help prevent the nozzle from cooling down 
# and stimulate flow, Enabling it can prevent blobs from sticking together. Values range 
# from 0 .. 1, or -1 if you don't want it changed.
#variable_part_cooling_fan: -1              # Leave it unchanged
#variable_part_cooling_fan:  0              # Disable the fan
variable_part_cooling_fan: 1               # Run it at full speed


# ========================================================================================
# ==================== PURGE LENGTH TUNING ===============================================
# ========================================================================================

# Default purge length to fall back on when neither the tool map purge_volumes or 
# parameter PURGE_LENGTH is set.
variable_purge_length: 61.1

# The absolute minimum to purge, even if you don't changed tools. This is to prime the 
# nozzle before printing
variable_purge_length_minimum: 50.99

# The slicer values often are a bit too wastefull. Tune it here to get optimal values. 0.6
# is a good starting point.
variable_purge_length_modifier: 1

# Length of filament to add after the purge volume. Purge volumes don't always take 
# cutters into account and therefor a swap from red to white might be long enough, but 
# from white to red can be far too short. When should you alter this value:
#   INCREASE: When the dark to light swaps are good, but light to dark aren't.
#   DECREASE: When the light to dark swaps are good, but dark to light aren't. Don't 
#     forget to increase the purge_length_modifier
variable_purge_length_addition: 0

# ========================================================================================
# ==================== BUCKET ============================================================
# ========================================================================================

# Maximum number of blobs that fit in the bucket. Pauses the print if it exceeds this 
# number.
variable_max_blobs: 400
# Enable the bucket shaker. You need to have the shaker.stl installed
variable_enable_shaker: 1
# The number of back-and-forth motions of one shake
variable_bucket_shakes: 10
# During shaking acceleration can often be higher because you don't neeed to keep print 
# quality in mind. Higher acceleration helps better with dispersing the blobs.
variable_shake_accel: 10000

# The frequency at which to shake the bucket. A decimal value ranging from 0 to 1, where 0 
# is never, and 1 is every time. This way the shaking occurs more often as the bucket 
# fills up. Sensible values range from 0.75 to 0.95
variable_bucket_shake_frequency: 0.95

gcode:

  # ======================================================================================
  # ==================== RECORD STATE (INCL. FANS, SPEEDS, ETC...) =======================
  # ======================================================================================

  # General state
  SAVE_GCODE_STATE NAME=BLOBIFIER_state

  # Part cooling fan
  {% if part_cooling_fan >= 0 %}
    # Save the part cooling fan speed to be enabled again later
    {% set backup_fan_speed = printer.fan.speed %}
    # Set part cooling fan speed
    M106 S{part_cooling_fan * 255}
  {% endif %}

  # Set feedrate to 100% for correct speed purging
  {% set backup_feedrate = printer.gcode_move.speed_factor %}
  M220 S100
  
  # ======================================================================================
  # ==================== CHECK HOMING STATUS =============================================
  # ======================================================================================
  
  {% if "xyz" not in printer.toolhead.homed_axes %}
    {action_raise_error("BLOBIFIER: Not homed! Home xyz first")}
  {% endif %}
  
  {% if printer.quad_gantry_level and printer.quad_gantry_level.applied == False %}
    {action_raise_error("BLOBIFIER: QGL not applied!")}
  {% endif %}

  # ======================================================================================
  # ==================== DEFINE BASIC VARIABLES ==========================================
  # ======================================================================================
  
  {% set filament_diameter = printer.configfile.config.extruder.filament_diameter|float %}
  {% set filament_cross_section = (filament_diameter/2) ** 2 * 3.1415 %}
  {% set from_tool = printer.mmu.last_tool %}
  {% set to_tool = printer.mmu.tool %}
  {% set bl_count = printer['gcode_macro _BLOBIFIER_COUNT'] %}
  {% set pos = printer.gcode_move.gcode_position %}
  {% set restore_z = [printer['gcode_macro BLOBIFIER_PARK'].restore_z,pos.z]|max %}
  {% set pos_max = printer.toolhead.axis_maximum %}
  {% set position_y = pos_max.y %}

  # Get purge volumes from the slicer (if set up right. see 
  # https://github.com/moggieuk/Happy-Hare/blob/variables/doc/gcode_preprocessing.md)
  {% set pv = printer.mmu.slicer_tool_map.purge_volumes %}
  
  # ======================================================================================
  # ==================== DETERMINE PURGE LENGTH ==========================================
  # ======================================================================================

  {% if params.PURGE_LENGTH %} # ======= PARAM PURGE LENGTH ==============================
    {action_respond_info("BLOBIFIER: param PURGE_LENGTH provided")}
    {% set purge_len = params.PURGE_LENGTH|float %}
  {% elif from_tool == to_tool and to_tool >= 0 %} # ==== TOOL DIDN'T CHANGE =============
    {action_respond_info("BLOBIFIER: Tool didn't change (T%s > T%s), %s" % (from_tool, to_tool, "priming" if purge_length_minimum else "skipping"))}
    {% set purge_len = 0 %}
  {% elif pv %} # ====================== FECTH FROM SLICER ===============================
    {% if from_tool < 0 and to_tool >= 0%}
      {action_respond_info("BLOBIFIER: from tool unknown. Finding largest value for T? > T%d" % to_tool)}
      {% set purge_vol = pv|map(attribute=to_tool)|max %}
    {% elif to_tool < 0 %}
      {action_respond_info("BLOBIFIER: tool(s) unknown. Finding largest value")}
      {% set purge_vol = pv|map('max')|max %}
    {% else %}
      {% set purge_vol = pv[from_tool][to_tool]|float * purge_length_modifier %}
      {action_respond_info("BLOBIFIER: Swapped T%s > T%s" % (from_tool, to_tool))}
    {% endif %}
    {% set purge_len = purge_vol / filament_cross_section + purge_length_addition %}
  {% else %} # ========================= USE CONFIG VARIABLE =============================
    {action_respond_info("BLOBIFIER: No toolmap or PURGE_LENGTH. Using default")}
    {% set purge_len = purge_length|float %}
  {% endif %}
  # ==================================== APPLY PURGE MINIMUM =============================
  {% set purge_len = [purge_len,purge_length_minimum]|max %}
  {action_respond_info("BLOBIFIER: Purging %.2fmm of filament" % (purge_len|float))}

  # ======================================================================================
  # ==================== PURGING SEQUENCE ================================================
  # ======================================================================================

  # Set to absolute positioning.
  G90

  # Check for purge length and purge if necessary.
  {% if purge_len|float > 0 %}

    # ====================================================================================
    # ==================== POSITIONING ===================================================
    # ====================================================================================
    
    # Move to the assembly, first a bit more to the right (brush_start) to avoid a 
    # potential filametrix pin if it's not already on the same Y coordinate.
    {% if printer.toolhead.position.y != position_y %}
      G1 X{brush_start} Y{position_y} F{travel_spd_xy}
    {% endif %}
    # Move over to the tray
    G1 X{purge_x} F{travel_spd_xy}

    # ====================================================================================
    # ==================== BUCKET SHAKE ==================================================
    # ====================================================================================
    
    {% if enable_shaker %}
      {% if (bl_count.current_blobs + 1) >= bl_count.next_shake %}
        BLOBIFIER_SHAKE_BUCKET SHAKES={bucket_shakes}
        _BLOBIFIER_CALCULATE_NEXT_SHAKE
      {% endif %}
    {% endif %}
    
    # ====================================================================================
    # ==================== POSITIONING ON TRAY ===========================================
    # ====================================================================================
    G1 Z{tray_top + purge_start} F{travel_spd_z}

    # Extend the blobifier
    BLOBIFIER_SERVO POS=out
    # Check if the servo is in correct position
    {% if printer.mmu is defined and printer.mmu.enabled %}
      {% if printer.configfile.config.mmu.sync_to_extruder %}
        MMU_SYNC_GEAR_MOTOR SYNC=1
      {% else %}
        MMU_SERVO POS=up
      {% endif %}
    {% endif %}

    # ====================================================================================
    # ==================== HEAT HOTEND ===================================================
    # ====================================================================================
    
    {% if printer.extruder.temperature < purge_temp_min %}
      {% if printer.extruder.target < purge_temp_min %}
        M109 S{purge_temp_min}
      {% else %}
        TEMPERATURE_WAIT SENSOR=extruder MINIMUM={purge_temp_min}
      {% endif %}
    {% endif %}

    # ====================================================================================
    # ==================== START ITERATING ===============================================
    # ====================================================================================
    
    # Calculate total number of iterations based on the purge length and the max_iteration 
    # length.
    {% set iterations = (purge_len / max_iteration_length)|round(0, 'ceil')|int %}

    # Repeat the process until purge_len is reached
    {% for n in range(iterations) %}
    
      # Calculate current iteration in current blob
      {% set step = n % max_iterations_per_blob %}

      {% if step == 0 %}
        G1 Z{tray_top + purge_start} F{travel_spd_z}
      {% endif %}

      # Determine the amount to extrude. Either the remaining purge_len or 
      # max_iteration_length.
      {% set purge_amount_left = purge_len - (max_iteration_length * n) %}
      {% set extrude_amount = [purge_amount_left,max_iteration_length]|min %}
      {% set extrude_ratio = extrude_amount / max_iteration_length %}

      # relative positioning
      G91 
      M83

      # Calculate the amount z has to be raised
      # If step equals 0, the start position of the nozzle is already a little higher.
      {% set step_triangular = step * (step + 1) / 2 %}
      {% set z_raise_substract = purge_start if step == 0 else 
        step_triangular * iteration_z_change %}
      {% set raise_z = (iteration_z_raise - z_raise_substract) * extrude_ratio %}

      # make sure raise_z never goes negative, dropping the nozzle while purging.
      {% set raise_z = [raise_z,0]|max %}

      # Calculate the raise speed based on the purge speed.
      {% set duration = extrude_amount / purge_spd %} 
      {% set speed = raise_z / duration %}
    
      # Purge one iteration
      G1 Z{raise_z} E{extrude_amount} F{speed}

      # ==================================================================================
      # ==================== DEPOSIT BLOB ================================================
      # ==================================================================================
      {% set max_iterations_reached = step == max_iterations_per_blob - 1 %}
      {% set purge_length_reached = purge_len - max_iteration_length * (n+1) <= 0 %}
      {% if max_iterations_reached or purge_length_reached %}
        # Raise z a bit to relieve pressure on the blob preventing it to go sideways
        G1 Z{eject_hop} F{travel_spd_z}
        # Retract the tray
        BLOBIFIER_SERVO POS=in
        # Move the toolhead down to purge_start height lowering the blob below the tray
        G90 # absolute positioning
        G1 Z{tray_top} F{travel_spd_z}
        # Extend the tray to 'cut off' the blob and prepare for the next blob
        BLOBIFIER_SERVO POS=out
        BLOBIFIER_SERVO POS=in
        BLOBIFIER_SERVO POS=out
        # Keep track of the # of blobs
        _BLOBIFIER_COUNT
      {% endif %}
    {% endfor %}
  {% endif %}
  G1 Z{tray_top + 1} F{travel_spd_z}
  G4 P{pressure_release_time}
  BLOBIFIER_CLEAN

  # ======================================================================================
  # ==================== RESTORE STATE ===================================================
  # ======================================================================================

  G1 Z{restore_z} F{travel_spd_z}
  
  {% if part_cooling_fan >= 0 %}
    # Reset part cooling fan if it was changed
    M106 S{backup_fan_speed * 255}
  {% endif %}
  
  M220 S{backup_feedrate * 100}
  
  RESTORE_GCODE_STATE NAME=BLOBIFIER_state 

##########################################################################################
# Wipes the nozzle on the brass brush
#
[gcode_macro BLOBIFIER_CLEAN]
gcode:
  {% set bb = printer['gcode_macro BLOBIFIER'] %}
  {% set position_y = printer.configfile.config["stepper_y"]["position_max"]|float %}
  # Position for wipe. Either left or right of brush based off bucket_pos to avoid 
  # unnecessary travel.
  
  SAVE_GCODE_STATE NAME=BLOBIFIER_CLEAN_state

  G90

  G1 Z{bb.brush_top + bb.clearance_z} F{bb.travel_spd_z}
  G1 X{bb.brush_start} F{bb.travel_spd_xy}
  G1 Y{position_y}

  # Move nozzle down into brush.
  G1 Z{bb.brush_top} F{bb.travel_spd_z}
   
  # Perform wipe. Wipe direction based off bucket_pos for cool random scrubby routine.
  {% for wipes in range(1, (bb.wipe_qty + 1)) %}
     G1 X{bb.brush_start + bb.brush_width} F{bb.wipe_spd_xy}
     G1 X{bb.brush_start} F{bb.wipe_spd_xy}
  {% endfor %}

  RESTORE_GCODE_STATE NAME=BLOBIFIER_CLEAN_state



##########################################################################################
# Park the nozzle on the tray to prevent oozing during filament swaps. Place this 
# extension in the pre-unload extension in mmu_macro_vars.cfg:
#   variable_user_pre_unload_extension: "BLOBIFIER_PARK"
#
[gcode_macro BLOBIFIER_PARK]
variable_restore_z: 0
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set pos = printer.gcode_move.gcode_position %}
  {% set pos_max = printer.toolhead.axis_maximum %}
  {% set position_y = pos_max.y %}

  SET_GCODE_VARIABLE MACRO=BLOBIFIER_PARK VARIABLE=restore_z VALUE={pos.z}

  SAVE_GCODE_STATE NAME=blobifier_park_state
  
  {% if "xy" not in printer.toolhead.homed_axes %}
    RESPOND MSG="Automatically homing XY"
    G28 X Y
  {% endif %}
  
  {% if "z" not in printer.toolhead.homed_axes %}
    RESPOND MSG="Automatically homing Z"
    G28 Z
  {% endif %}
  
  {% if printer.quad_gantry_level and printer.quad_gantry_level.applied == False %}
    RESPOND MSG="Automatically QGL"
    quad_gantry_level
    G28 Z
  {% endif %}

  G90

  G1 E-{bl.retract_before_park} F{bl.retract_speed}
  G1 X{bl.brush_start} Y{position_y} F{bl.travel_spd_xy}
  G1 X{bl.purge_x}
  G1 Z{bl.tray_top} F{bl.travel_spd_z}

  RESTORE_GCODE_STATE NAME=blobifier_park_state

##########################################################################################
# Retract or extend the tray 
# POS=[in|out] Retractor extend the tray
#
[gcode_macro BLOBIFIER_SERVO]
# increase this value if the servo doesn't have enough time to fully retract or extend
variable_dwell_time: 200
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set pos = params.POS %}
  {% if pos == "in" %}
    SET_SERVO SERVO=blobifier ANGLE={bl.tray_angle_in}
    G4 P{dwell_time}
  {% elif pos == "out" %}
    SET_SERVO SERVO=blobifier ANGLE={bl.tray_angle_out}
    G4 P{dwell_time}
  {% else %}
    {action_respond_info("BLOBIFIER: provide POS=[in|out]")}
  {% endif %}
  SET_SERVO SERVO=blobifier WIDTH=0

##########################################################################################
# Increment the blob count with 1 and check if the bucket is full. Pause 
# the printer if it is.
#
[gcode_macro _BLOBIFIER_COUNT]
# Don't change these variables
variable_current_blobs: 0
variable_last_shake: 0
variable_next_shake: 0
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set count = printer['gcode_macro _BLOBIFIER_COUNT'] %}
  {% if current_blobs >= bl.max_blobs %}
    {action_respond_info("BLOBIFIER: Empty purge bucket!")}
    M117 Empty purge bucket!
    PAUSE
  {% else %}
    SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=current_blobs VALUE={current_blobs + 1}
    _BLOBIFIER_SAVE_STATE
    {action_respond_info(
      "BLOBIFIER: Blobs in bucket: %s/%s. Next shake @ %s" 
      % (current_blobs + 1, bl.max_blobs, next_shake)
    )}
  {% endif %}

##########################################################################################
# Reset the blob count to 0
#
[gcode_macro _BLOBIFIER_COUNT_RESET]
gcode:
  SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=current_blobs VALUE=0
  SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=last_shake VALUE=0
  _BLOBIFIER_SAVE_STATE
  
  _BLOBIFIER_CALCULATE_NEXT_SHAKE

##########################################################################################
# Shake the blob bucket to disperse the blobs
#
[gcode_macro BLOBIFIER_SHAKE_BUCKET]
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set count = printer['gcode_macro _BLOBIFIER_COUNT'] %}
  {% set original_accel = printer.toolhead.max_accel %}
  {% set original_minimum_cruise_ratio = printer.toolhead.minimum_cruise_ratio %}
  
  {% if "xyz" not in printer.toolhead.homed_axes %}
    {action_raise_error("BLOBIFIER: Not homed. Home xyz first")}
  {% endif %}
  
  SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=last_shake VALUE={count.current_blobs}
  _BLOBIFIER_SAVE_STATE
  SAVE_GCODE_STATE NAME=shake_bucket
  
  M400
  M117 (^_^)

  G90
  {% set shakes = params.SHAKES|default(10)|int %}
  {% set position_y = printer.configfile.config["stepper_y"]["position_max"]|float %}
  
  # move to save y if not already there
  {% if printer.toolhead.position.y != position_y %}
    G1 X{bl.brush_start} Y{position_y} F{bl.travel_spd_xy}
  {% endif %}

  # move up a bit to prevent oozing on base
  G1 Z2 F{bl.travel_spd_z}
  # slide into the slot
  G1 X0 F{bl.travel_spd_xy}

  M400
  M117 (+(+_+)+)

  SET_VELOCITY_LIMIT ACCEL={bl.shake_accel} MINIMUM_CRUISE_RATIO=0.1
  
  # Shake away!
  {% for shake in range(1, shakes) %}
     G1 Y{position_y - 4}
     G1 Y{position_y}
  {% endfor %}

  SET_VELOCITY_LIMIT ACCEL={original_accel} MINIMUM_CRUISE_RATIO={original_minimum_cruise_ratio}
  # move out of slot
  G1 X{bl.purge_x}

  M400
  M117 (X_x)

  RESTORE_GCODE_STATE NAME=shake_bucket  

##########################################################################################
# Calculate when the bucket should be shaken. 
#
[gcode_macro _BLOBIFIER_CALCULATE_NEXT_SHAKE]
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set count = printer['gcode_macro _BLOBIFIER_COUNT'] %}

  {% set remaining_blobs = bl.max_blobs - count.last_shake %}
  {% set next_shake = (1 - bl.bucket_shake_frequency) * remaining_blobs + count.last_shake %}
  _BLOBIFIER_SAVE_STATE
  _BLOBIFIER_SET_NEXT_SHAKE VALUE={next_shake|int}

##########################################################################################
# Set when the bucket should be shaken next
# VALUE=[int] At what amount of blobs should it be shaken
#
[gcode_macro _BLOBIFIER_SET_NEXT_SHAKE]
gcode:
  {% if params.VALUE %}
    {% set next_shake = params.VALUE %}
    SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=next_shake VALUE={next_shake}
    _BLOBIFIER_SAVE_STATE
  {% else %}
    {action_respond_info("BLOBIFIER: Provide parameter VALUE=")}
  {% endif %}

##########################################################################################
# Some sanity checks
#
[delayed_gcode BLOBIFIER_INIT]
initial_duration: 5.0
gcode:
  _BLOBIFIER_INIT
  BLOBIFIER_SERVO POS=out

[gcode_macro _BLOBIFIER_INIT]
gcode:
  {% set bl = printer['gcode_macro BLOBIFIER'] %}
  {% set raise_z_min = (bl.iteration_z_raise - ((bl.max_iterations_per_blob - 1) * (bl.max_iterations_per_blob) / 2) * bl.iteration_z_change) %}
  
  # No drop on purging
  {% if raise_z_min < 0 %} 
    {action_emergency_stop("BLOBIFIER: Your current blob settings cause the nozzle to drop on the final iteration(s). increase iteration_z_raise or decrease either iteration_z_change or max_iterations_per_blob.")}
  {% endif %}

  # Valid part cooling fan setting
  {% if bl.part_cooling_fan != -1 and (bl.part_cooling_fan < 0 or bl.part_cooling_fan > 1) %}
    {action_emergency_stop("BLOBIFIER: Value %f is invalid for variable part_cooling_fan. Either -1 or a value from 0 .. 1 is valid." % (bl.part_cooling_fan))}
  {% endif %}

  # Valid bucket shake frequency
  {% if bl.bucket_shake_frequency < 0 or bl.bucket_shake_frequency > 1 %}
    {action_emergency_stop("BLOBIFIER: Value %f is invalid for variable bucket_shake_frequency. Change it to a value between 0 .. 1" % (bl.bucket_shake_frequency))}
  {% endif %}  

  {% if printer.mmu %}
    {% if printer['gcode_macro _MMU_SEQUENCE_VARS'].restore_xy_pos != 'next' %}
      {action_respond_info("BLOBIFIER: If not using a wipe tower, consider setting restore_xy_pos: 'next' in mmu_macro_vars.cfg")}
    {% endif %}

    # Blobifier park enabled
    {% if printer['gcode_macro _MMU_SEQUENCE_VARS'].user_pre_unload_extension == "BLOBIFIER_PARK" %}

      # MMU park is enabled
      {% if printer['gcode_macro _MMU_SEQUENCE_VARS'].enable_park %}
        {action_respond_info("BLOBIFIER: MMU park is enabled in mmu_macro_vars.cfg. This could interfere with BLOBIFIER_PARK")}
        
        # MMU Park after form tip is enabled
        {% if printer['gcode_macro _MMU_SEQUENCE_VARS'].park_after_form_tip %}
          {action_respond_info("BLOBIFIER: MMU park_after_form_tip in mmu_macro_vars.cfg is enabled and could interfere with BLOBIFIER_PARK")}
        {% endif %}
      {% endif %}

      # Filametrix installed?
      {% if printer.configfile.config.mmu.form_tip_macro == '_MMU_CUT_TIP' %}

        # Restore position after CUT disabled
        {% if not printer['gcode_macro _MMU_CUT_TIP_VARS'].restore_position %}
          {action_respond_info("BLOBIFIER: _MMU_CUT_TIP restore_position in mmu_macro_vars.cfg is disabled, this disables BLOBIFIER_PARK's use")}
        {% endif %}
    
        # Tray_top too low
        {% if bl.tray_top <= 0.2 %}
          {action_emergency_stop("BLOBIFIER: Both BLOBIFIER_PARK and _MMU_CUT_TIP are enabled, but the tray_top is too low (< 0.2). This will cause the nozzle to run into the bed during cutting")}
        {% endif %}
    
      {% endif %}
    
    # Blobifier enabled, but Blobifier_park isn't
    {% elif printer['gcode_macro _MMU_SEQUENCE_VARS'].user_post_load_extension == 'BLOBIFIER' %}
      {action_respond_info("BLOBIFIER: Consider setting user_pre_unload_extension in mmu_macro_vars.cfg to BLOBIFIER_PARK")}
    {% endif %}
  {% endif %}


[delayed_gcode BLOBIFIER_LOAD_STATE]
initial_duration: 2.0 # Give it some time to boot up
gcode:
  {% set sv = printer.save_variables.variables.blobifier %}

  {% if sv %}
    # Restore state
    SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=last_shake VALUE={sv.last_shake}
    SET_GCODE_VARIABLE MACRO=_BLOBIFIER_COUNT VARIABLE=current_blobs VALUE={sv.current_blobs}
  {% endif %}
  _BLOBIFIER_CALCULATE_NEXT_SHAKE

[gcode_macro _BLOBIFIER_SAVE_STATE]
gcode:
  {% set count = printer['gcode_macro _BLOBIFIER_COUNT'] %}
  {% set sv = {'current_blobs': count.current_blobs, 'last_shake': count.last_shake} %}
  SAVE_VARIABLE VARIABLE=blobifier VALUE="{sv}"

