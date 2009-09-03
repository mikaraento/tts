cd ..\group
cd group
call abld target gcce urel
call abld target gcce udeb
cd ..\sis
makesis tts_proto_fep.pkg
makesis tts_proto_fep_dbg.pkg
signsis tts_proto_fep.SIS tts_proto_fep-signed.SIS \software\jaiku-private\devcert\mfgr\jaiku3rd.cer \software\jaiku-private\devcert\jaiku.key
signsis tts_proto_fep_dbg.SIS tts_proto_fep_dbg-signed.SIS \software\jaiku-private\devcert\mfgr\jaiku3rd.cer \software\jaiku-private\devcert\jaiku.key
copy *.SIS f:\
