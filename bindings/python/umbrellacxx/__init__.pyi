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
from typing import TypeVar, Iterable

from . import objc

__version__: str = ...
__commit__: str = ...
__tag__: str = ...
__full_version__: str = ...

class InProcess:
    def __init__(self, __address: int) -> None: ...
    @property
    def address(self) -> int: ...
    def apply_relative_offset(self, offset: int) -> int: ...

T = TypeVar("T")

class it(Iterable[T]):
    def __next__(self) -> T: ...
    def __len__(self) -> int: ...
    def __getitem__(self, __index: int, /) -> T: ...

class ABIBase:
    @property
    def image_base(self) -> int: ...
