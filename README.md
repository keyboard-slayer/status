# Status
## Overview
### Description
As you may know, DWM has no status bar...

So I did my own

### Building the Project
Run `sudo make install` and voilà 

### Usage with DWM
Add those lines to your `$HOME/.xinitrc`

```bash
while true; do
    xsetroot -name "$(status)"
    sleep 20s # Update every 20 seconds
done &
```

