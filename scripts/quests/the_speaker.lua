if get_num_deities() == 0 then
  add_message_with_pause("THE_SPEAKER_GODSLAYER_SPEECH_TEXT_SID")
  clear_and_add_message("THE_SPEAKER_GODSLAYER_SPEECH_TEXT2_SID")
else
  add_message_with_pause("THE_SPEAKER_SPEECH_TEXT_SID")
  add_message_with_pause("THE_SPEAKER_SPEECH_TEXT2_SID")
  add_message_with_pause("THE_SPEAKER_SPEECH_TEXT3_SID")
  add_message_with_pause("THE_SPEAKER_SPEECH_TEXT4_SID")
  clear_and_add_message("THE_SPEAKER_SPEECH_TEXT5_SID")
end

