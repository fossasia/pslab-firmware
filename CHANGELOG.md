# Changelog

## [3.1.0] - 2025-02-17

### Changed

- Increase UART timeout ([`26de862`](https://github.com/fossasia/pslab-firmware/commit/26de86294997252c3c171e8be93c92410e57d354)) (Anashuman Singh)

### Added

- Document how to build for v5 hardware ([`1a5da5a`](https://github.com/fossasia/pslab-firmware/commit/1a5da5acb5eba67291d622cb9e4953167786c91d)) (Marc Nause)

## [3.0.4] - 2025-01-05

### Fixed

- Fix ACKSTAT missing from response in several I2C commands ([`6356eb2`](https://github.com/fossasia/pslab-firmware/commit/6356eb2656f856327c9090bcbd6e404db78ae2f0)) (Alexander Bessman)

## [3.0.3] - 2024-08-19

### Fixed

- Fix UART2 pin mappings not matching labels ([`5728b36`](https://github.com/fossasia/pslab-firmware/commit/5728b36939f8dd76bfd83889c1cd94a57bab4329)) (Alexander Bessman)

## [3.0.2] - 2024-07-30

### Added

- Add changelog (Alexander Bessman)

### Fixed

- Fix `I2C_CommandReadBulk` reading one too many bytes ([`4ea85ec`](https://github.com/fossasia/pslab-firmware/commit/4ea85ec9ecda2f9ec4dcab0b56f0de5edb6fbfaa)) (Alexander Bessman)

## [3.0.1] - 2024-07-15

### Fixed

- Fix broken capacitance measurement ([`36d4fd3`](https://github.com/fossasia/pslab-firmware/commit/36d4fd31fe6edc3845e16ab71af899f61262b061)) (Alexander Bessman)
- Fix second half of sample buffer not being cleared ([`54034b8`](https://github.com/fossasia/pslab-firmware/commit/54034b81549d735af3ab5050bdcd06a08269a6b2)) (Alexander Bessman)

## [3.0.0] - 2024-01-15

_Major refactorization of firmware._

[3.1.0]: https://github.com/fossasia/pslab-firmware/releases/tag/3.1.0
[3.0.4]: https://github.com/fossasia/pslab-firmware/releases/tag/3.0.4
[3.0.3]: https://github.com/fossasia/pslab-firmware/releases/tag/3.0.3
[3.0.2]: https://github.com/fossasia/pslab-firmware/releases/tag/3.0.2
[3.0.1]: https://github.com/fossasia/pslab-firmware/releases/tag/3.0.1
[3.0.0]: https://github.com/fossasia/pslab-firmware/releases/tag/v3.0.0
