-- ============= SOKU2 config  =============
-- Available languages: en, jp
set_language("en")








-- =========================================
-- it is not recommended to change anything below this line
-- =========================================

set_nextsoku_path("./NextSoku.dll")
set_characters_dir("./characters")
set_debug_mode(false)

-- =========================================
-- available vanilla soku characters
Reimu 		= 0
Marisa 		= 1
Sakuya		= 2
Alice		= 3
Patchouli	= 4
Youmu		= 5
Remilia		= 6
Yuyuko		= 7
Yukari		= 8
Suika		= 9
Reisen		= 10
Aya			= 11
Komachi		= 12
Iku			= 13
Tenshi		= 14
Sanae		= 15
Cirno		= 16
Meiling		= 17
Utsuho		= 18
Suwako		= 19
Reversed	= 20
Reversed	= 21

-- SOKU2 version
set_version("2.30f")
-- add custom characters
-- Momiji    = add_character(22, "momiji",     Reimu,     "momiji.dat",     "momiji.lua",     "momiji.asm",     "deck.cfg") --22 - 40
-- Clownpiece= add_character(23, "clownpiece", Reisen,    "clownpiece.dat", "clownpiece.lua", "clownpiece.asm", "deck.cfg") --23 - 30 ? 50
-- Flandre   = add_character(24, "flandre",    Remilia,   "flandre.dat",    "flandre.lua",    "flandre.asm",    "deck.cfg") --24 - 26 ? 46
-- Orin      = add_character(25, "orin",       Cirno,     "orin.dat",       "orin.lua",       "orin.asm",       "deck.cfg") --25 - 36
-- Yuuka     = add_character(26, "yuuka",      Utsuho,    "yuuka.dat",      "yuuka.lua",      "yuuka.asm",      "deck.cfg") --26 - 38
-- Kaguya    = add_character(27, "kaguya",     Patchouli, "kaguya.dat",     "kaguya.lua",     "kaguya.asm",     "deck.cfg") --27 - 24 ? 44
-- Mokou     = add_character(28, "mokou",      Aya,       "mokou.dat",      "mokou.lua",      "mokou.asm",      "deck.cfg") --28 - 31 ? 51
-- Mima      = add_character(29, "mima",       Iku,       "mima.dat",       "mima.lua",       "mima.asm",       "deck.cfg") --29 - 33 ? 53
-- Shou      = add_character(30, "shou",       Tenshi,    "shou.dat",       "shou.lua",       "shou.asm",       "deck.cfg") --30 - 34 ? 54
-- Murasa    = add_character(31, "murasa",     Suika,     "murasa.dat",     "murasa.lua",     "murasa.asm",     "deck.cfg") --31 - 29 ? 49
-- Sekibanki = add_character(32, "sekibanki",  Youmu,     "sekibanki.dat",  "sekibanki.lua",  "sekibanki.asm",  "deck.cfg") --32 - 25 ? 45
-- Satori    = add_character(33, "satori",     Yuyuko,    "satori.dat",     "satori.lua",     "satori.asm",     "deck.cfg") --33 - 27 ? 47
-- Ran       = add_character(34, "ran",        Sakuya,    "ran.dat",        "ran.lua",        "ran.asm",        "deck.cfg") --34 - 22 ? 42
-- Shinki    = add_character(39, "shinki",     Suwako,    "shinki.dat",     "shinki.lua",     "shinki.asm",     "deck.cfg") --35 - 39
--{{EXTRA_CHARACTERS}}

--always skip 22-35; these IDs are used in pre-giuroll and might hold incorrect card IDs
--scratch that
--in any case, for characters after Ran, we use modulo 20

-- add common resources
append_dat("characters/common/common.dat")
--append_dat("stages/moon.dat")

-- set order in charselect and deck builder
character_list = {
  Reimu,
  Marisa,
  Sakuya,
  Alice,
  Patchouli,
  Youmu,
  Remilia,
  Yuyuko,
  Yukari,
  Suika,
  Reisen,
  Aya,
  Komachi,
  Iku,
  Tenshi,
  Sanae,
  Cirno,
  Meiling,
  Utsuho,
  Suwako,
  -- Momiji,
  -- Clownpiece,
  -- Flandre,
  -- Orin,
  -- Yuuka,
  -- Kaguya,
  -- Mokou,
  -- Mima,
  -- Shou,
  -- Murasa,
  -- Sekibanki,
  -- Satori,
  -- Ran,
  --Shinki,
  --{{EXTRA_CHARACTERS_ORDER}}
}

--[[

character_list =
{
  Sekibanki,
  Shou,
  Mokou,
  Yuuka,
  Flandre,
  Momiji,
  Suika,
  Yukari,
  Yuyuko,
  Remilia,
  Youmu,
  Patchouli,
  Alice,
  Sakuya,
  Marisa,
  Reimu,
  Sanae,
  Cirno,
  Meiling,
  Utsuho,
  Suwako,
  Reisen,
  Aya,
  Komachi,
  Iku,
  Tenshi,
  Clownpiece,
  Orin,
  Kaguya,
  Mima,
  Murasa,
  Satori
  --Shinki
}


34;ran;Ran;Ran Yakumo;214,623,22,236
34;nue;Nue;Nue Houjuu;236,623,22,214
35;shinki;Shinki;Shinki;214,623,236,22

]]--

set_select_order(character_list)
set_deck_order(character_list)
