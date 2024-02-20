import { MenuItem, Icon, Menu, MenuHeader } from 'semantic-ui-react'
import { useState } from 'react';

export default function FileExplorer(
  { fileExplorerActive }: { fileExplorerActive: boolean }
) {
  const [files, setFiles] = useState<string[]>([]);

  if (!fileExplorerActive) {
    return null;
  }

  return (
    <div>
      <Menu
        icon='labeled'
        vertical
        floated={true}
        style={{ display: 'flex', height: '100%', marginTop: 0 }}
      >
        <MenuHeader>
          File Explorer
        </MenuHeader>
        <MenuItem>
        </MenuItem>
      </Menu>
    </div>
  );
}
