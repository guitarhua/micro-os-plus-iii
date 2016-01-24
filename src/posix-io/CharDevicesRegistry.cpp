/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/CharDevicesRegistry.h"
#include "posix-io/CharDevice.h"
#include <cmsis-plus/diag/trace.h>

#include <cassert>
#include <cstring>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    std::size_t CharDevicesRegistry::sfSize;

    CharDevice** CharDevicesRegistry::sfRegistryArray;

    // ------------------------------------------------------------------------

    CharDevicesRegistry::CharDevicesRegistry (std::size_t size)
    {
      assert(size > 0);

      sfSize = size;
      sfRegistryArray = new CharDevice*[size];

      for (std::size_t i = 0; i < size; ++i)
        {
          sfRegistryArray[i] = nullptr;
        }
    }

    CharDevicesRegistry::~CharDevicesRegistry ()
    {
      delete[] sfRegistryArray;
      sfSize = 0;
    }

    // ------------------------------------------------------------------------

    void
    CharDevicesRegistry::add (CharDevice* device)
    {
#if defined(DEBUG)
      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] == nullptr)
            {
              continue;
            }

          // Validate the device name by checking duplicates.
          if (std::strcmp (device->getName (), sfRegistryArray[i]->getName ())
              == 0)
            {
              trace_puts ("Duplicate Device name. Abort.");
              std::abort ();
            }

        }
#endif // DEBUG

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] == nullptr)
            {
              sfRegistryArray[i] = device;
              return;
            }
        }

      trace_puts ("Max number of Devices reached. Abort.");
      std::abort ();
    }

    void
    CharDevicesRegistry::remove (CharDevice* device)
    {
      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] == device)
            {
              sfRegistryArray[i] = nullptr;
              return;
            }
        }

      // Not found... It would be good to tell.
    }

    /**
     * return pointer to device or nullptr if not found.
     */
    CharDevice*
    CharDevicesRegistry::identifyDevice (const char* path)
    {
      assert(path != nullptr);

      auto prefix = CharDevice::getDevicePrefix ();
      if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
        {
          // The device prefix does not match, not a device.
          return nullptr;
        }

      // The prefix was identified; try to match the rest of the path.
      auto name = path + std::strlen (prefix);
      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfRegistryArray[i] != nullptr
              && sfRegistryArray[i]->matchName (name))
            {
              // Return the first device that matches the path.
              return sfRegistryArray[i];
            }
        }

      // Not a known device.
      return nullptr;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
