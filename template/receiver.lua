return block
{
      name="receiver",
      meta_data="",
      port_cache=true,

      types = {
	 { name="structsender_data", class='struct' }, -- Enum will follow once implemented in C
	 --{ name="receiver_info", class='struct' },
      },

      ports = {
	 { name="data", in_type_name="struct structsender_data", doc="struct containing string and number" },
      },
      
      operations = { start=false, stop=false, step=true }
}
