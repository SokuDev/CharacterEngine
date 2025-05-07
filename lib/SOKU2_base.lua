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
Reimu    = 0
Marisa   = 1
Sakuya   = 2
Alice    = 3
Patchouli= 4
Youmu    = 5
Remilia  = 6
Yuyuko   = 7
Yukari   = 8
Suika    = 9
Reisen   = 10
Aya      = 11
Komachi  = 12
Iku      = 13
Tenshi   = 14
Sanae    = 15
Cirno    = 16
Meiling  = 17
Utsuho   = 18
Suwako   = 19
Random   = 20
Namazu   = 21

-- SOKU2 version
set_version("2.30h")
-- add custom characters
--{{EXTRA_CHARACTERS}}

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
  --{{EXTRA_CHARACTERS_DEFINITION}}
}

character_list_select =
{
  --{{EXTRA_CHARACTERS_BEFORE}}
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
  --{{EXTRA_CHARACTERS_AFTER}}
}


set_select_order(character_list_select)
set_deck_order(character_list)
