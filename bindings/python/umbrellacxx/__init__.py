# Copyright (c) 2023 MatrixEditor
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import sys

if len(__path__) > 0 and __path__[0] not in sys.path:
    from . import _umbrella
    from ._umbrella import *
    from ._umbrella import __version__, __commit__, __tag__, __full_version__

    # see https://github.com/pytorch/pytorch/blob/60a3b7425dde97fe8b46183c154a9c3b24f0c733/torch/__init__.py#L467-L470
    for attribute in dir(_umbrella):
        candidate = getattr(_umbrella, attribute)
        if type(candidate) == type(_umbrella):
            sys.modules.setdefault(f"umbrella.{attribute}", candidate)
