# Changelog

## [1.0.5](https://github.com/mdvorak/esphome-adaptive-lighting/compare/v1.0.4...v1.0.5) (2025-07-03)


### Bug Fixes

* adjust elevation if out of range for today ([87d8d6c](https://github.com/mdvorak/esphome-adaptive-lighting/commit/87d8d6c0842e6f38abbaaa1816e06fc0d656e2f8))

## [1.0.4](https://github.com/mdvorak/esphome-adaptive-lighting/compare/v1.0.3...v1.0.4) (2025-05-23)


### Bug Fixes

* fix compatibility with ESPHome 2025.5 ([96f0fa2](https://github.com/mdvorak/esphome-adaptive-lighting/commit/96f0fa201daf3eb1bb5fbab0906094c55056117e))

## [1.0.3](https://github.com/mdvorak/esphome-adaptive-lighting/compare/v1.0.2...v1.0.3) (2025-03-02)


### Bug Fixes

* avoid warnings about color out of range ([9c6a829](https://github.com/mdvorak/esphome-adaptive-lighting/commit/9c6a829b2ba7d2626d7b56c4f2d10f1e40471708))

## [1.0.2](https://github.com/mdvorak/esphome-adaptive-lighting/compare/v1.0.1...v1.0.2) (2025-02-02)


### Bug Fixes

* set color temp even when off, to have smooth turn-on transition ([9852860](https://github.com/mdvorak/esphome-adaptive-lighting/commit/9852860383caf7d71c7240f61606690f456ccc05))

## [1.0.1](https://github.com/mdvorak/esphome-adaptive-lighting/compare/v1.0.0...v1.0.1) (2025-01-30)


### Bug Fixes

* dump config as debug only ([ad61f41](https://github.com/mdvorak/esphome-adaptive-lighting/commit/ad61f41fe8190ff799f6f3d9ed8dce3255afc28a))

## 1.0.0 (2025-01-30)

This is initial release of `adaptive-lightning` component. Supported features are:

* Color temperature adjustments according to current sun elevation
* External change detection
