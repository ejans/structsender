return pkg
{
  name="microblx_structsender",
  path="../",
      
  dependencies = {
    --{ name="kdl-types", type="cmake" },
  },
  
  types = {
    { name="structsender_data", dir="types" },
  },
  
  blocks = {
    { name="sender", file="structsender_init/sender.lua", src_dir="src" },
    { name="receiver", file="structsender_init/receiver.lua", src_dir="src" },
  },
  
  libraries = {
    { name="sender", blocks={"sender"} },
    { name="receiver", blocks={"receiver"} },
--     { name="anotherlib", blocks={"second_block"} } , 
  },
}
