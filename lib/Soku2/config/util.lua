function class(cl)
  cl.__index = cl
  function cl.new(...)
    local instance = {}
    setmetatable(instance, cl)
    if cl.init then cl.init(instance, ...) end
    return instance
  end
  return cl
end

function new(cl, ...)
  return cl.new(...)
end

local Config = class({
  init = function(self)
    self.values = {}
  end,

  get = function(self, ...)
    local arg = {...}
    return self.values[table.concat(arg, "_")]
  end,

  getdef = function(self, ...)
    local arg = {...}
    local def = arg[#arg]
    table.remove(arg)
    return self.values[table.concat(arg, "_")] or def
  end,

  set = function(self, ...)
    local arg = {...}
    local v = arg[#arg]
    table.remove(arg)
    local k = table.concat(arg, "_")
    self.values[k] = v
    set_config_option(k, v)
  end,

  run = function(self, pat, f, ...)
    local arg = {...}
    local v = self.values[table.concat(arg, "_")]
    if v then
      f(pat, v)
    end
  end
})

function table.shallow_copy(tab)
  local res = {}
  for k, v in pairs(tab) do
    res[k] = v
  end
  return res
end

function table.map(tab, func)
  local res = {}
  for k, v in pairs(tab) do
    res[k] = func(v)
  end
  return res
end

function table.foreach(tab, func)
  for k, v in pairs(tab) do
    if type(v) ~= 'table' then
      func(v)
    else
      func(table.unpack(v))
    end
  end
end

function table.keys(tab)
  local res = {}
  for k, v in pairs(tab) do
    table.insert(res, k)
  end
  return res
end

function table.sorted_keys(tab)
  local keys = table.keys(tab)
  table.sort(keys)
  return keys
end

function table.tostring(t, compact)
  if type(t) ~= 'table' then error(tostring(t) .. ': not a table') end
  local seen = {[t]=true}
  local sep = compact and ',' or ', '
  local eq = compact and '=' or ' = '
  local open, close = '{', '}'

  local function strval(rec, e)
    if type(e) == 'table' then
      if seen[e] then
        return '<cycle>'
      else
        seen[e] = true
        return rec(e)
      end
    else
      return tostring(e)
    end
  end

  local function rec(t)
    local r = open
    local it, arg, i = ipairs(table.sorted_keys(t))
    local i, k = it(arg, i)
    while k do
      local v = t[k]
      if type(k) == 'number' then
        r = r .. strval(rec, v)
      else
        r = r .. strval(rec, k) .. eq .. strval(rec, v)
      end
      i, k = it(arg, i)
      if k then r = r .. sep end
    end
    return r .. close
  end
  return rec(t)
end

function table.apply(t, f)
  f(table.unpack(t))
end

function table.merge(t, f)
  for k, v in pairs(f) do t[k] = f[k] end
end

function table.merge_or_apply(t, f)
  if type(f) == 'function' then
    f(t)
  else
    table.merge(t, f)
  end
end

table.__tostring = table.tostring

function load_local_module(module_name)
  return dofile(string.format("%s/NextSoku/config/%s", os.getenv("LOCALAPPDATA"), module_name))
end

function load_modset_module(modset, module_name)
  return dofile(string.format("%s", module_name))
end

function load_pattern_generator(charname, gendir)
  local success, result = pcall(function()
    return load_modset_module(
      config:get("global", "modset_name"),
	  string.format("%s/%s.lua", (gendir or "characters/" .. charname), charname))
      --string.format("%s/%s.lua", (gendir or "gen"), charname))
  end)

  if success then
    return result
  end

  msgbox("Warning: " .. result)
  return nil
end

function combine_generators(charname, a, b)
  return function(pat, charname)
    if a then a(pat, charname) end
    if b then b(pat, charname) end
  end
end

local MessageBox = class({
  __call = function(self, object)
    if type(object) == 'table' then  
      return show_message_box(table.tostring(object))
    end

    show_message_box(tostring(object))
  end,

  format = function(fmt, ...)
    show_message_box(string.format(fmt, ...))
  end
})

-- global variable
msgbox = new(MessageBox)

function copy_sprite(self, src)
  self.tex_index = src.tex_index
  self.tex_x = src.tex_x
  self.tex_y = src.tex_y 
  self.tex_w = src.tex_w 
  self.tex_h  = src.tex_h
  self.tex_offset_x = src.tex_offset_x 
  self.tex_offset_y = src.tex_offset_y
end

function Sequence:append_frames_of(src)
  local base = self:get_frame_count()
  for idx = 0, src:get_frame_count() - 1 do
    self:add_frame(base + idx, src:get_frame(idx))
  end
end

function Sequence:update_frame(idx, changes)
  local frame = self:get_frame(idx)
  table.merge_or_apply(frame, changes)
  self:set_frame(idx, frame)
end

function PatternData:update_frame(seq, offset, idx, changes)
  local frame = self:get_frame(seq, offset, idx)
  table.merge_or_apply(frame, changes)
  self:set_frame(seq, offset, idx, frame)
end

function PatternData:update_range(seq, offset, first, last, changes)
  for idx = first, last do
    self:update_frame(seq, offset, idx, changes)
  end
end

function PatternData:add_frame(seq, offset, idx, frame)
  self:get_sequence(seq, offset):add_frame(idx, frame)
end

function PatternData:remove_frame(seq, offset, idx)
  self:get_sequence(seq, offset):remove_frame(idx)
end

function PatternData:split_frame(seq, offset, idx, duration_1, duration_2)
  local frame = self:get_frame(seq, offset, idx)
  frame.duration = duration_2
  self:set_frame(seq, offset, idx, frame)
  frame.duration = duration_1
  self:add_frame(seq, offset, idx, frame)
end

function PatternData:get_frame_count(seq, offset)
  return self:get_sequence(seq, offset):get_frame_count()
end

function PatternData:copy_sequence(new_id, src_id, n_children)
  for offset = 0, n_children do
    local src = self:get_sequence(src_id, offset)
    self:add_sequence(new_id, offset, src:is_looped(), src:get_priority())
    self:get_sequence(new_id, offset):append_frames_of(src)
  end
end

function PatternData:has_sequence(seq)
  local pat = self
  local success, _ = pcall(function() pat:get_sequence(seq, 0) end)
  return success
end

function grow_box(box, left, right, top, bottom)
  box.x1 = box.x1 - left
  box.x2 = box.x2 + right
  box.y1 = box.y1 - top
  box.y2 = box.y2 + bottom
end

function grow_box_sym(box, x, y)
  grow_box(box, x, x, y, y)
end

_register_pattern_generator = register_pattern_generator

function register_pattern_generator(charname, gen)
  local pre = config:get("system", "pattern_generator", "before")
  local post = config:get("system", "pattern_generator", "after")
  _register_pattern_generator(charname, function(pat)
    if pre then pre(pat, charname) end
    if gen then gen(pat, charname) end
    if post then post(pat, charname) end
  end)
end

-- todo: remove
PatternData.modify_frame = PatternData.update_frame

function set_fflag(frame, flag)
  frame.fflags = bit32.bor(frame.fflags, flag)
end

function unset_fflag(frame, flag)
  frame.fflags = bit32.band(frame.fflags, bit32.bnot(flag))
end

function set_aflag(frame, flag)
  frame.aflags = bit32.bor(frame.aflags, flag)
end

function unset_aflag(frame, flag)
  frame.aflags = bit32.band(frame.aflags, bit32.bnot(flag))
end

function set_motion_cancel(frame)
  set_fflag(frame, FF_MOTION_CANCEL)
end

function set_attack_cancel(frame)
  set_fflag(frame, FF_ATTACK_CANCEL)
end

function set_attack_and_motion_cancel(frame)
  set_fflag(frame, FF_ATTACK_CANCEL + FF_MOTION_CANCEL)
end

function set_sprite(src)
  return function(frame)
    copy_sprite(frame, src)
  end
end



-- fear's custom funcs


function PatternData:add_particle_info(seq, offset, idx)
  self:update_frame(seq, offset, idx, { data_format = 2 , blending_mode = 0 , color_a = 255, color_r = 255, color_g = 255, color_b = 255 , scale_x = 100, scale_y = 100, angle_x = 0, angle_y = 0, angle_z = 0 })
end
  
  --pat:get_sequence(520,2):set_looped(false)
--






-- attack flags
AF_STAGGER                  = 0x1
AF_HIGHBLOCK_OK             = 0x2
AF_LOWBLOCK_OK              = 0x4
AF_AIR_BLOCKABLE            = 0x8
AF_GROUND_UNBLOCKABLE       = 0x10
AF_IGNORE_ARMOR             = 0x20
AF_HIT_ON_WRONGBLOCK        = 0x40      
AF_GRAB                     = 0x80   
AF_UNKNOWN_1                = 0x100
AF_UNKNOWN_2                = 0x200
AF_COUNTER_HIT              = 0x400
AF_SKILL_ATTACK             = 0x800
AF_SPELL_ATTACK             = 0x1000
AF_ATTACK_AIR               = 0x2000
AF_ATTACK_HIGH              = 0x4000
AF_ATTACK_LOW               = 0x8000
AF_ATTACK_OBJECT            = 0x10000
AF_UNREFLECTABLE            = 0x20000
AF_UNKNOWN_4                = 0x40000
AF_GUARD_CRUSH              = 0x80000
AF_CAN_HIT_SELF             = 0x100000
AF_PROJECTILE               = 0x400000
AF_UNGRAZABLE               = 0x800000
AF_DRAINS_ON_GRAZE          = 0x1000000

-- frame flags
FF_STANDING                 = 0x1
FF_CROUCHING                = 0x2
FF_AIRBORNE                 = 0x4
FF_UNKNOWN_1                = 0x8
FF_GUARD_CANCEL             = 0x10
FF_CANCEL                   = 0x20
FF_ATTACK_CANCEL            = 0x20
FF_COUNTER_HIT              = 0x40
FF_SUPER_ARMOR              = 0x80
FF_EXTENDED_ARMOR           = 0x100
FF_GUARD_POINT              = 0x200
FF_GRAZE                    = 0x400
FF_GUARDING                 = 0x800
FF_GRAB_INV                 = 0x1000
FF_MELEE_INV                = 0x2000
FF_BULLET_INV               = 0x4000
FF_COUNTER_AIR              = 0x8000
FF_COUNTER_HIGH             = 0x10000
FF_COUNTER_LOW              = 0x20000
FF_COUNTER_BULLET           = 0x40000
FF_REFLECT_BULLET           = 0x80000
FF_FLIP_VELOCITY            = 0x100000
FF_MOTION_CANCEL            = 0x200000
FF_AFTER_IMAGE              = 0x400000
FF_LOOP_ANIMATION           = 0x800000
FF_ATTACK_OBJECT            = 0x1000000

-- attack levels
AL_LIGHT = 0
AL_MEDIUM = 1
AL_HEAVY = 2
AL_SUPERHEAVY = 3

-- combo correction
CC_NONE = 0
CC_LIFT_ATTACK = 1
CC_SMASH_ATTACK = 2
CC_BORDER_RESIST = 4
CC_CHAIN_ARTS = 8
CC_CHAIN_SPELL = 16
CC_COUNTER_HIT = 32

-- globals
config = Config.new()