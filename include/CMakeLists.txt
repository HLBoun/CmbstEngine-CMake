

set(INCLUDED
      cmbst_window.hpp
      first_app.hpp
      cmbst_pipeline.hpp
      cmbst_device.hpp
      cmbst_game_object.hpp
      cmbst_model.hpp
      cmbst_swap_chain.hpp
      cmbst_renderer.hpp

)

add_library(myInclude
      STATIC
      ${INCLUDED}
)
