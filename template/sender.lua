return block
{
      name="sender",
      meta_data="",
      port_cache=true,

      types = {
	 { name="structsender_data", class='struct' }, -- Enum will follow once implemented in C
	 --{ name="sender_info", class='struct' },
      },

      configurations= {
	 { name="string", type_name="char", doc="string to send out" },
	 { name="number", type_name="int", doc="number to send out" },
      },

      ports = {
	 { name="data", out_type_name="struct structsender_data", doc="struct containing string and number" },
      },
      
      operations = { start=true, stop=true, step=true }
}
